#include "oakpch.h"
#include "oak/utilities/FileSystem.h"

#include "oak/core/Application.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <commdlg.h>
#include <Shlobj.h>


namespace Oak {

    float Time::GetTime()
	{
		return (float)glfwGetTime();
	}

	FileSystem::FileSystemChangedCallbackFn FileSystem::s_Callback;

	static bool s_Watching = false;
	static bool s_IgnoreNextChange = false;
	static HANDLE s_WatcherThread;

	void FileSystem::SetChangeCallback(const FileSystemChangedCallbackFn& callback)
	{
		s_Callback = callback;
	}

	void FileSystem::StartWatching()
	{
		s_Watching = true;
		DWORD threadId;
		s_WatcherThread = CreateThread(NULL, 0, Watch, 0, 0, &threadId);
		CORE_ASSERT(s_WatcherThread != NULL);
		SetThreadDescription(s_WatcherThread, L"Hazel FileSystemWatcher");
	}

	void FileSystem::StopWatching()
	{
		if (!s_Watching)
			return;

		s_Watching = false;
		//DWORD result = WaitForSingleObject(s_WatcherThread, 5000);
		//if (result == WAIT_TIMEOUT)
		// NOTE(Yan): this is a little annoying, but it's a quick and dirty
		//            way to shutdown the file watching ASAP.
		TerminateThread(s_WatcherThread, 0);
		CloseHandle(s_WatcherThread);
	}

	std::filesystem::path FileSystem::OpenFileDialog(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			std::string fp = ofn.lpstrFile;
			std::replace(fp.begin(), fp.end(), '\\', '/');
			return std::filesystem::path(fp);
		}
		return std::filesystem::path();
	}

	std::filesystem::path FileSystem::OpenFolderDialog(const char* initialFolder)
	{
		std::filesystem::path result = "";
		IFileOpenDialog* dialog;
		if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&dialog)))
		{
			DWORD options;
			dialog->GetOptions(&options);
			dialog->SetOptions(options | FOS_PICKFOLDERS);
			if (SUCCEEDED(dialog->Show(NULL)))
			{
				IShellItem* selectedItem;
				if (SUCCEEDED(dialog->GetResult(&selectedItem)))
				{
					PWSTR pszFilePath;
					if (SUCCEEDED(selectedItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFilePath)))
					{
						result = std::filesystem::path(pszFilePath, std::filesystem::path::generic_format);
						CoTaskMemFree(pszFilePath);
					}

					selectedItem->Release();
				}
			}

			dialog->Release();
		}

		std::string fp = result.string();
		std::replace(fp.begin(), fp.end(), '\\', '/');
		return fp;
	}

	std::filesystem::path FileSystem::SaveFileDialog(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			std::string fp = ofn.lpstrFile;
			std::replace(fp.begin(), fp.end(), '\\', '/');
			return std::filesystem::path(fp);
		}

		return std::filesystem::path();
	}

	void FileSystem::SkipNextFileSystemChange()
	{
		s_IgnoreNextChange = true;
	}

	unsigned long FileSystem::Watch(void* param)
	{
		//auto assetDirectory = Project::GetActive()->GetAssetDirectory();
		std::filesystem::path assetDirectory = std::filesystem::path("assets/");
		std::wstring dirStr = assetDirectory.wstring();

		char buf[2048];
		DWORD bytesReturned;
		std::filesystem::path filepath;
		BOOL result = TRUE;

		HANDLE directoryHandle = CreateFile(
			dirStr.c_str(),
			GENERIC_READ | FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			NULL
		);

		if (directoryHandle == INVALID_HANDLE_VALUE)
		{
			CORE_VERIFY(false, "Failed to open directory!");
			return 0;
		}

		OVERLAPPED pollingOverlap;
		pollingOverlap.OffsetHigh = 0;
		pollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		std::vector<FileSystemChangedEvent> eventBatch;
		eventBatch.reserve(10);

		while (s_Watching && result)
		{
			result = ReadDirectoryChangesW(
				directoryHandle,
				&buf,
				sizeof(buf),
				TRUE,
				FILE_NOTIFY_CHANGE_FILE_NAME |
				FILE_NOTIFY_CHANGE_DIR_NAME |
				FILE_NOTIFY_CHANGE_SIZE,
				&bytesReturned,
				&pollingOverlap,
				NULL
			);

			WaitForSingleObject(pollingOverlap.hEvent, INFINITE);

			if (s_IgnoreNextChange)
			{
				s_IgnoreNextChange = false;
				eventBatch.clear();
				continue;
			}

			FILE_NOTIFY_INFORMATION* pNotify;
			int offset = 0;
			std::wstring oldName;

			do
			{
				pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buf + offset);
				size_t filenameLength = pNotify->FileNameLength / sizeof(wchar_t);

				FileSystemChangedEvent e;
				e.FilePath = std::filesystem::path(std::wstring(pNotify->FileName, filenameLength));
				e.IsDirectory = IsDirectory(e.FilePath);

				switch (pNotify->Action)
				{
				case FILE_ACTION_ADDED:
				{
					e.Action = FileSystemAction::Added;
					break;
				}
				case FILE_ACTION_REMOVED:
				{
					e.Action = FileSystemAction::Delete;
					break;
				}
				case FILE_ACTION_MODIFIED:
				{
					e.Action = FileSystemAction::Modified;
					break;
				}
				case FILE_ACTION_RENAMED_OLD_NAME:
				{
					oldName = e.FilePath.filename();
					break;
				}
				case FILE_ACTION_RENAMED_NEW_NAME:
				{
					e.OldName = oldName;
					e.Action = FileSystemAction::Rename;
					break;
				}
				}

				// NOTE(Peter): Fix for https://gitlab.com/chernoprojects/Hazel-dev/-/issues/143
				bool hasAddedEvent = false;
				if (e.Action == FileSystemAction::Modified)
				{
					for (const auto& event : eventBatch)
					{
						if (event.FilePath == e.FilePath && event.Action == FileSystemAction::Added)
							hasAddedEvent = true;
					}
				}

				if (pNotify->Action != FILE_ACTION_RENAMED_OLD_NAME && !hasAddedEvent)
					eventBatch.push_back(e);

				offset += pNotify->NextEntryOffset;
			} while (pNotify->NextEntryOffset);

			if (eventBatch.size() > 0)
			{
				s_Callback(eventBatch);
				eventBatch.clear();
			}
		}

		CloseHandle(directoryHandle);
		return 0;
	}

} 
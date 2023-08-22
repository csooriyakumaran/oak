#include "oakpch.h"
#include "oak/core/Application.h"

#include "oak/renderer/Renderer.h"

#include "oak/core/Input.h"
#include "oak/utilities/FileSystem.h"

#include "imgui.h"

extern bool g_ApplicationRunning;
extern ImGuiContext* GImGui;

namespace Oak {


    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& specification)
        : m_Specification(specification) 
    {
		s_Instance = this;
        // set working directory
        if (m_Specification.WorkingDirectory.empty()) 
			m_Specification.WorkingDirectory = std::filesystem::current_path().string();
		
		LOG_CORE_WARN("{}", m_Specification.WorkingDirectory);
		
		WindowSpecification windowSpec;
		windowSpec.Title      = specification.Name;
		windowSpec.Width      = specification.WindowWidth;
		windowSpec.Height     = specification.WindowHeight;
		windowSpec.Decorated  = specification.WindowDecorated;
		windowSpec.Fullscreen = specification.Fullscreen;
		windowSpec.VSync      = specification.VSync;
		
		m_Window = std::unique_ptr<Window>(Window::Create(windowSpec));
		m_Window->Init();
		m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

		Renderer::Init();
		// m_RenderThreat.pump()

		if (specification.StartMaximized)
			m_Window->Maximize();
		else
			m_Window->CenterWindow();
		
		m_Window->SetResizable(specification.Resizable);


		m_ImGuiBaseLayer = new ImGuiBaseLayer;
		PushLayer(m_ImGuiBaseLayer);
        
    }

    Application::~Application()
    {
		m_Window->SetEventCallback([](Event& e) {});
		for (Layer* layer : m_LayerStack)
		{
			LOG_CORE_INFO_TAG("~Application", "Deleting Layer {0}", layer->GetName().c_str());
			layer->OnDetach();
			delete layer;
		}
		// m_RenderThread.Terminate();
		Renderer::Shutdown();
    }
	void Application::CreateNewWindow()
	{
	}

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
		layer->OnAttach();

    }

	 void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
		layer->OnAttach();

    }


   	void Application::OnEvent(Event& e)
	{

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(OAK_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(OAK_BIND_EVENT_FN(Application::OnWindowResize));
		dispatcher.Dispatch<WindowRestoreEvent>(OAK_BIND_EVENT_FN(Application::OnWindowRestore));
		dispatcher.Dispatch<WindowMinimizeEvent>(OAK_BIND_EVENT_FN(Application::OnWindowMinimize));
		dispatcher.Dispatch<WindowMaximizeEvent>(OAK_BIND_EVENT_FN(Application::OnWindowMaximize));

		for (auto& eventCallback : m_EventCallbacks)
		{
			eventCallback(e);

			if (e.Handled)
				break;
		}

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled) 
				break;
			(*it)->OnEvent(e);
		}
		if (e.Handled)
			return;

	}

    
	void Application::Run()
	{
		// main thread
		OnInit(); // client implemented initilization
		while (m_Running)
		{
			// add main thread profiling here


			static uint64_t frameCounter = 0;
			// LOG_CORE_INFO("--- BEGIN FRAME {0}", frameCounter);
			ProcessEvents(); 

			if (!m_Minimized)
			{
				{
					//add scope profiling and threading
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(m_TimeStep);
				}

				m_ImGuiBaseLayer->Begin();
				{
					//create a render thread
					for (Layer* layer : m_LayerStack)
						layer->OnUIRender();
				}
				m_ImGuiBaseLayer->End();
			}

			m_Window->OnUpdate();


			float time = Time::GetTime();
			m_TimeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			// LOG_CORE_INFO("--- END FRAME {0}", frameCounter);
			frameCounter++;

		}
		OnShutdown();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnShutdown()
	{
		m_EventCallbacks.clear();
		g_ApplicationRunning = false;
	}

	void Application::ProcessEvents()
	{
		//m_Window->ProcessEvents(); // does this need to be implemented?

		std::scoped_lock<std::mutex> lock(m_EventQueueMutex);

		// Process custom event queue
		while (m_EventQueue.size() > 0)
		{
			auto& func = m_EventQueue.front();
			func();
			m_EventQueue.pop();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return false; // give other things a chance to react to the window closing
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		LOG_CORE_ERROR_TAG("Application::OnWindowResize", "h {0}, w {1}", e.GetHeight(), e.GetWidth());

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
	bool Application::OnWindowMinimize(WindowMinimizeEvent& e)
	{
		LOG_CORE_ERROR_TAG("Application::OnWindowMinimize", "{0}", e);
		m_Minimized = true;
		m_Maximized = false;
		m_Restored  = false;
		m_Window->Minimize();
		return true;
	}
	bool Application::OnWindowMaximize(WindowMaximizeEvent& e)
	{
		
		LOG_CORE_ERROR_TAG("Application::OnWindowMaximize", "{0}", e);
		m_Minimized = false;
		m_Maximized = true;
		m_Restored = false;
		m_Window->Maximize();
		return true;
	}
	bool Application::OnWindowRestore(WindowRestoreEvent& e)
	{
		LOG_CORE_ERROR_TAG("Application::OnWindowRestore", "{0}", e);
		m_Minimized = false;
		m_Maximized = false;
		m_Restored  = true;
		m_Window->Restore();
		return false;
	}


}
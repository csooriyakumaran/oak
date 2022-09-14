#include "oakpch.h"
#include "oak/core/Application.h"
#include "oak/core/Base.h"

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
        if (m_Specification.workingDirectory.empty()) 
			m_Specification.workingDirectory = std::filesystem::current_path().string();
		
		OAK_CORE_WARN("{}", m_Specification.workingDirectory);


        m_Window = Window::Create(WindowProps(m_Specification.name));
        m_Window->SetEventCallback(OAK_BIND_EVENT_FN(Application::OnEvent));
		m_Window->Maximize();

		Renderer::Init();

		m_ImGuiBaseLayer = new ImGuiBaseLayer;
		PushLayer(m_ImGuiBaseLayer);
        
    }

    Application::~Application()
    {
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

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled) 
				break;
			(*it)->OnEvent(e);
		}
	}

    
	void Application::Run()
	{
		OnInit(); // client implemented initilization
		while (m_Running)
		{
			float time = Time::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			//ProcessEvents(); // process deferred events

			if (!m_Minimized)
			{
				{
					//add scope profiling
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiBaseLayer->Begin();
				{
					for (Layer* layer : m_LayerStack)
						layer->OnUIRender();
				}
				m_ImGuiBaseLayer->End();
			}

			m_Window->OnUpdate();
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

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
	bool Application::OnWindowMinimize(WindowMinimizeEvent& e)
	{
		m_Minimized = true;
		m_Maximized = false;
		m_Restored  = false;
		return false;
	}
	bool Application::OnWindowMaximize(WindowMaximizeEvent& e)
	{
		m_Minimized = false;
		m_Maximized = true;
		m_Restored = false;
		return false;
	}
	bool Application::OnWindowRestore(WindowRestoreEvent& e)
	{
		m_Minimized = false;
		m_Maximized = false;
		m_Restored  = true;
		return false;
	}


}
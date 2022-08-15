#include "oakpch.h"
#include "oak/core/Application.h"
#include "oak/core/Base.h"

#include "oak/renderer/Renderer.h"

#include "oak/core/Input.h"
#include "oak/utils/PlatformUtils.h"


namespace Oak {


    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& specification)
        : m_Specification(specification)
    {
		s_Instance = this;
        // set working directory
        if (!m_Specification.workingDirectory.empty()) 
            std::filesystem::current_path(m_Specification.name);

        m_Window = Window::Create(WindowProps(m_Specification.name));
        m_Window->SetEventCallback(OAK_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer;
		PushLayer(m_ImGuiLayer);
        
    }

    Application::~Application()
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


    void Application::Close()
	{
		m_Running = false;
	}

   	void Application::OnEvent(Event& e)
	{

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(OAK_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(OAK_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled) 
				break;
			(*it)->OnEvent(e);
		}
	}

    
	void Application::Run()
	{

		while (m_Running)
		{

			float time = Time::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					for (Layer* layer : m_LayerStack)
						layer->OnUIRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		// Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}


}
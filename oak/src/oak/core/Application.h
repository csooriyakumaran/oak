#pragma once

#include "oakpch.h"

#include "oak/core/Window.h"
#include "oak/core/LayerStack.h"

#include "oak/events/Event.h"
#include "oak/events/ApplicationEvent.h"


#include "Oak/core/Timestep.h"


namespace Oak {
    int Main(int argc, char** argv);

    struct ApplicationSpecification
	{
		std::string name = "Oak App";
        std::string workingDirectory;
		std::string iconPath = "";
	};

    class Application
    {
    public:
        Application(const ApplicationSpecification& specification);
        ~Application();

        void OnEvent(Event& e);
        void PushLayer(Layer* layer);

        Window& GetWindow() { return *m_Window;}

        void Close();

        static Application& get() { return *s_Instance;}

        const ApplicationSpecification& getSpecification() const { return m_Specification;}

    private:
        void Run();
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        ApplicationSpecification m_Specification;
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

    private:
        static Application* s_Instance;
        friend int Oak::Main(int argc, char** argv);


    };

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);


}
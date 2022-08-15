#pragma once

#include "oakpch.h"
#include "oak/core/Base.h"

#include "oak/core/Window.h"
#include "oak/core/LayerStack.h"

#include "oak/events/Event.h"
#include "oak/events/ApplicationEvent.h"

#include "oak/imgui/ImGuiLayer.h"


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
        void PushOverlay(Layer *layer);

        Window& GetWindow() { return *m_Window;}

        void Close();

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        static Application& Get() { return *s_Instance;}

        const ApplicationSpecification& GetSpecification() const { return m_Specification;}

    private:
        void Run();
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        ApplicationSpecification m_Specification;
        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

    private:
        static Application* s_Instance;
        friend int Main(int argc, char** argv);


    };

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);


}
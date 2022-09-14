#pragma once

#include "oakpch.h"
#include "oak/core/Base.h"

#include "oak/core/Window.h"
#include "oak/core/LayerStack.h"

#include "oak/events/Event.h"
#include "oak/events/ApplicationEvent.h"

#include "oak/imgui/ImGuiBaseLayer.h"


#include "Oak/core/Timestep.h"


namespace Oak {
    int Main(int argc, char** argv);

    struct ApplicationSpecification
	{
		std::string name = "Oak App";
        std::string workingDirectory;
        bool fullscreen = true;
	};


 


    class Application
    {
        using EventCallbackFn = std::function<void(Event&)>;

    public:
        Application(const ApplicationSpecification& specification);
        ~Application();

        void Run();
        void Close();

        virtual void OnInit() {}
        virtual void OnShutdown();
        virtual void OnUpdate(Timestep ts) {}

        virtual void OnEvent(Event& e);
        
        void PushLayer(Layer* layer);
        void PushOverlay(Layer *layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

        void RenderImGui();

        void AddEventCallBack(const EventCallbackFn& eventCallback) { m_EventCallbacks.push_back(eventCallback); }


        inline Window& GetWindow() { return *m_Window;}
        
        bool IsRunning() {return m_Running;}
        bool IsMinimized() {return m_Minimized;}
        bool IsMaximized() {return m_Maximized;}
        bool IsRestored() { return m_Restored; }

        ImGuiBaseLayer* GetImGuiBaseLayer() { return m_ImGuiBaseLayer; }

        static Application& Get() { return *s_Instance;}
        const ApplicationSpecification& GetSpecification() const { return m_Specification;}

        void CreateNewWindow();

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        bool OnWindowMinimize(WindowMinimizeEvent& e);
        bool OnWindowMaximize(WindowMaximizeEvent& e);
        bool OnWindowRestore(WindowRestoreEvent& e);

    private:
        ApplicationSpecification m_Specification;
        Scope<Window> m_Window;
        ImGuiBaseLayer* m_ImGuiBaseLayer;
        LayerStack m_LayerStack;
        std::vector<EventCallbackFn> m_EventCallbacks;
                
        bool m_Running = true;
        bool m_Minimized = false;
        bool m_Maximized = true;
        bool m_Restored = false;
        
        float m_LastFrameTime = 0.0f;

    private:
        static Application* s_Instance;
        friend int Main(int argc, char** argv);


    };

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);


}
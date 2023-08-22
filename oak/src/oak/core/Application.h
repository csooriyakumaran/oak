#pragma once

#include "oakpch.h"
#include "oak/core/Base.h"

#include "oak/core/Window.h"
#include "oak/core/LayerStack.h"

#include "oak/events/Event.h"
#include "oak/events/ApplicationEvent.h"

#include "oak/imgui/ImGuiBaseLayer.h"


namespace Oak {
    int Main(int argc, char** argv);



    struct ApplicationSpecification
	{
		std::string Name = "Wallace Instruments App";
        uint32_t WindowWidth = 1600;
        uint32_t WindowHeight = 900;
        bool WindowDecorated = false;
        bool Fullscreen = false;
        bool VSync = true;
        std::string WorkingDirectory;
        bool StartMaximized = true;
        bool Resizable = true;
        bool EnableImGui = true;
        // ThreadingPolicy CoreThreadingPolicy = ThreadingPolicy::MultiThreaded;
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
        virtual void OnUpdate(wi::Timestep ts) {}

        virtual void OnEvent(Event& e);
        
        void PushLayer(Layer* layer);
        void PushOverlay(Layer *layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

        void RenderImGui();

        void AddEventCallBack(const EventCallbackFn& eventCallback) { m_EventCallbacks.push_back(eventCallback); }

        template<typename Func>
        void QueEvent(Func&& func)
        {
            m_EventQueue.push(func);
        }

        // Creates & Dispatches and event either immediately, or adds it to the event queue which will be processed at the end of each frame
        template<typename TEvent, bool DispatchImmediately = false, typename... TEventArgs>
        void DispatchEvent(TEventArgs&&... args)
        {
            static_assert(std::is_assignable_v<Event, TEvent>);

            std::shared_ptr<TEvent> event = std::make_shared<TEvent>(std::forward<TEventArgs>(args)...);
            if constexpr (DispatchImmediately)
            {
                OnEvent(*event);
            }
            else
            {
                std::scoped_lock<std::mutex> lock(m_EventQueueMutex);
                m_EventQueue.push([event]() {Application::Get().OnEvent(*event); });
            }
        }

        inline Window& GetWindow() { return *m_Window;}
        
        bool IsRunning() {return m_Running;}
        bool IsMinimized() {return m_Minimized; }
        bool IsMaximized() {return m_Window->IsMaximized(); }
        bool IsRestored() { return m_Restored; }

        ImGuiBaseLayer* GetImGuiBaseLayer() { return m_ImGuiBaseLayer; }

        static Application& Get() { return *s_Instance;}
        const ApplicationSpecification& GetSpecification() const { return m_Specification;}

        void CreateNewWindow();

    private:
        void ProcessEvents();
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        bool OnWindowMinimize(WindowMinimizeEvent& e);
        bool OnWindowMaximize(WindowMaximizeEvent& e);
        bool OnWindowRestore(WindowRestoreEvent& e);

    private:
        ApplicationSpecification m_Specification;
        wi::Scope<Window> m_Window;
        ImGuiBaseLayer* m_ImGuiBaseLayer;
        LayerStack m_LayerStack;

        std::mutex m_EventQueueMutex;
        std::queue<std::function<void()>> m_EventQueue;
        std::vector<EventCallbackFn> m_EventCallbacks;
                
        bool m_Running = true;
        bool m_Minimized = false;
        bool m_Maximized = false;
        bool m_Restored = false;
        
        wi::Timestep m_TimeStep;
        wi::Timestep m_FrameTime;

        float m_CPUTime = 0.0f;
        float m_LastFrameTime = 0.0f;

    private:
        static Application* s_Instance;
        friend int Main(int argc, char** argv);


    };

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);


}
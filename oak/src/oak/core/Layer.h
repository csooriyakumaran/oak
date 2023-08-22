#pragma once

#include "oakpch.h"
#include "oak/events/Event.h"



namespace Oak {

	class Layer 
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(wi::Timestep ts) {}
		virtual void OnUIRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;

	};

}
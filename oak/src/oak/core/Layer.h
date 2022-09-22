#pragma once

#include "oak/core/Ref.h"
#include "oak/core/Timestep.h"
#include "oak/events/Event.h"



namespace Oak {

	class Layer 
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnUIRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;

	};

}
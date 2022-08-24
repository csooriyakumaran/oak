#pragma once

#include "oak/core/Layer.h"

#include "oak/events/ApplicationEvent.h"
#include "oak/events/KeyEvent.h"
#include "oak/events/MouseEvent.h"

namespace Oak {

	class ImGuiBaseLayer : public Layer
	{
	public:
		ImGuiBaseLayer();
		~ImGuiBaseLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;

	};

}

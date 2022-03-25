#pragma once

#include "Skye/Core/Layer.hpp"

#include "Skye/Events/ApplicationEvent.hpp"
#include "Skye/Events/MouseEvent.hpp"
#include "Skye/Events/KeyEvent.hpp"

namespace Skye {

	class SKYE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		
		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}



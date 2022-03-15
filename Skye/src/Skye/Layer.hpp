#pragma once

#include "Skye/Core.hpp"
#include "Skye/Events/Event.hpp"
#include "Skye/Core/Timestep.hpp"

namespace Skye {
	/* Represents a layer and should be subclassed, i.e. UI Layer */
	class SKYE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}

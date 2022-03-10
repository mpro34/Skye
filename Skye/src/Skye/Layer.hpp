#pragma once

#include "Skye/Core.hpp"
#include "Skye/Events/Event.hpp"

namespace Skye {
	/* Represents a layer and should be subclassed, i.e. UI Layer */
	class SKYE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}

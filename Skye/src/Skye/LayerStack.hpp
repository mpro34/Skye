#pragma once

#include "Skye/Core.hpp"
#include "Layer.hpp"

#include <vector>

namespace Skye {

	/* A wrapper over a vector of layers. The application owns the layer stack. 
		Overlays will always be after layers!
	*/
	class SKYE_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};

}

#pragma once

#include "MyRevoke/Core.h"
#include "Layer.h"



namespace Revoke {
	class RV_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
			
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);
		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_LayersArr.begin(); }
		std::vector<Layer*>::iterator end() { return m_LayersArr.end(); }
	private:
		std::vector<Layer*> m_LayersArr;
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}

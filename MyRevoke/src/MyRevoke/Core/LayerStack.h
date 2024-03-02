#pragma once

#include "Core.h"
#include "Layer.h"



namespace Revoke {
	class  LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();
			
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);
		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_LayersArr.begin(); }
		std::vector<Layer*>::iterator end() { return m_LayersArr.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_LayersArr.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_LayersArr.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_LayersArr.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_LayersArr.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_LayersArr.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_LayersArr.rend(); }
	private:
		std::vector<Layer*> m_LayersArr;
		unsigned int  m_LayerInsertInd = 0;
	};
}

#include "rvpch.h"
#include "LayerStack.h"


namespace Revoke
{
	LayerStack::LayerStack()
	{
	}
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_LayersArr)
			delete layer;
	}
	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_LayersArr.emplace_back(overlay);
	}
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_LayersArr.begin(), m_LayersArr.end(), overlay);
		
		if (it != m_LayersArr.end())
			m_LayersArr.erase(it);
	}
	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayersArr.emplace(m_LayersArr.begin() + m_LayerInsertInd, layer);
		m_LayerInsertInd++;
	}
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_LayersArr.begin(), m_LayersArr.end(), layer);

		if (it != m_LayersArr.end())
		{
			m_LayersArr.erase(it);
			m_LayerInsertInd--;
		}
	}
}
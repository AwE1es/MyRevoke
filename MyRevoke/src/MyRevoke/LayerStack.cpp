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
		overlay->OnAttach();
	}
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_LayersArr.begin(), m_LayersArr.end(), overlay);

			if (it != m_LayersArr.end())
			{
				m_LayersArr.erase(it);
				overlay->OnDetach();
			}
	}
	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayersArr.emplace(m_LayersArr.begin() + m_LayerInsertInd, layer);
		layer->OnAttach();
		m_LayerInsertInd++;
	}
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_LayersArr.begin(), m_LayersArr.end(), layer);

		if (it != m_LayersArr.end())
		{
			m_LayersArr.erase(it);
			layer->OnDetach();
			m_LayerInsertInd--;
		}
	}
}
#include "rvpch.h"
#include "LayerStack.h"


namespace Revoke
{

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_LayersArr)
		{
			layer->OnDetach();
			delete layer;
		}
	}
	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_LayersArr.emplace_back(overlay);
	}
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_LayersArr.begin() + m_LayerInsertInd, m_LayersArr.end(), overlay);

			if (it != m_LayersArr.end())
			{
				overlay->OnDetach();
				m_LayersArr.erase(it);
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
		auto it = std::find(m_LayersArr.begin(), m_LayersArr.begin() + m_LayerInsertInd, layer);

		if (it != m_LayersArr.begin() + m_LayerInsertInd)
		{
			layer->OnDetach();
			m_LayersArr.erase(it);
			m_LayerInsertInd--;
		}
	}
}
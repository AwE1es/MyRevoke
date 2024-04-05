#pragma once
#include <filesystem>

#include "MyRevoke/Renderer/Texture.h"
namespace Revoke
{

	class ContentBrowser
	{
	public:
		ContentBrowser();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrendDir;
		Shared<Texture2D> m_FolderIcon;
		Shared<Texture2D> m_FileIcon;
	};

}



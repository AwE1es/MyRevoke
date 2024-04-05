#pragma once

#include <filesystem>
namespace Revoke
{

	class ContentBrowser
	{
	public:
		ContentBrowser();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrendDir;
	};

}



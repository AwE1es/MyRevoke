#include "rvpch.h"
#include "ContentBrowser.h"

#include <imgui.h>


namespace Revoke
{
	static const std::filesystem::path s_AssetsDirectory = "assets";

	ContentBrowser::ContentBrowser()
		:m_CurrendDir(s_AssetsDirectory)
	{

	}

	void ContentBrowser::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrendDir != std::filesystem::path(s_AssetsDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrendDir = m_CurrendDir.parent_path();
			}
		}

		//Does not support multiple language!!!
		for (auto entryPath : std::filesystem::directory_iterator(m_CurrendDir))
		{
			std::string pathString = entryPath.path().string();
			auto relativePath = std::filesystem::relative(entryPath.path(), s_AssetsDirectory);
			std::string fileNameString = relativePath.filename().string();

			if(entryPath.is_directory())
			{
				if (ImGui::Button(fileNameString.c_str()))
				{
					m_CurrendDir /= entryPath.path().filename();
				}
			}
			else
			{
				if (ImGui::Button(fileNameString.c_str()))
				{
					
				}
			}
		}

		ImGui::End();
	}
}

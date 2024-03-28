#pragma once
#include <string>

namespace Revoke
{
	class FileExplorer
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}


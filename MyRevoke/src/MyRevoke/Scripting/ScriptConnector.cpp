#include "rvpch.h"
#include "ScriptConnector.h"

#include <mono/metadata/object.h>

namespace Revoke
{
	namespace Internal
	{
		static void CppFunction()
		{
			std::cout << "taken from cpp\n";
		}
	}

	void ScriptConnector::ConnectFunctions()
	{
		mono_add_internal_call("Revoke.InternalCalls::CppFunction", Internal::CppFunction);
	}
}


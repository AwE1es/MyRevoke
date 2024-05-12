#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <winnt.h>
#include <memory>

#pragma warning( disable : 4278 )
#pragma warning( disable : 4146 )
#import "libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2" version("8.0") lcid("0") raw_interfaces_only named_guids	//The following #import imports EnvDTE based on its LIBID.
#pragma warning( default : 4146 )
#pragma warning( default : 4278 )


typedef void* (*scripVoidPtr)();



namespace Revoke
{
	struct DLLdata
	{
		HMODULE		dllHandle;
		scripVoidPtr playerScript;
	};

	class ScriptEntity;

	class ScriptEngine
	{
	public:
		ScriptEngine();

		void OnUpdate();
		void ShutDown();

		ScriptEntity* GetScritpByName(std::string scriptName);
	private:
		void InitDll();
		void OnDllUpdate();
	private:
		bool m_IsTick = true;
		DLLdata		m_Dll;

		char m_ExePath[MAX_PATH];
		char m_DllPath[MAX_PATH];

		FILETIME m_DllLastWriteTime;
		FILETIME m_DllTimeNew;

		bool m_IsStartWithDebugger = false;

		char m_PatchedDllPath[MAX_PATH] = { '\0' };
		char m_PatchedPdbPath[MAX_PATH] = { '\0' };

		bool m_Ok;
	};
}
#include "rvpch.h"
#include "NativeScript.h"



namespace Revoke
{

	namespace Utilities
	{
		static void unloadDLL(DLLdata dll)
		{
			if (dll.dllHandle)
				FreeLibrary(dll.dllHandle);
		}
	}

	EnvDTE::Process* FindVSProcess(DWORD TargetPID)
	{
		HRESULT hr;

		static const wchar_t* ProgID = L"VisualStudio.DTE";

		CLSID Clsid;
		CLSIDFromProgID(ProgID, &Clsid);

		IUnknown* Unknown;
		hr = GetActiveObject(Clsid, 0, &Unknown);
		if (FAILED(hr))
			return nullptr;

		EnvDTE::_DTE* Interface;

		hr = Unknown->QueryInterface(&Interface);
		if (FAILED(hr))
			return nullptr;

		EnvDTE::Debugger* Debugger;
		hr = Interface->get_Debugger(&Debugger);
		if (FAILED(hr))
			return nullptr;

		EnvDTE::Processes* Processes;
		hr = Debugger->get_LocalProcesses(&Processes);
		if (FAILED(hr))
			return nullptr;

		long Count = 0;
		hr = Processes->get_Count(&Count);
		if (FAILED(hr))
			return nullptr;

		EnvDTE::Process* Result = nullptr;
		for (int i = 1; i < Count; ++i)	// index 0 is invalid, hr == DISP_E_BADINDEX
		{
			EnvDTE::Process* Process;

			// get the process, but sometimes may fail,
			// so we re-try a number of times to get it...

			const int	retryTime = 5000;	// ms
			const int	retryInterval = 10;	// ms
			const int	retryCntMax = retryTime / retryInterval;
			int			retryCnt = 0;
			do
			{
				hr = Processes->Item(variant_t(i), &Process);
				if (FAILED(hr))	// usually return RPC_E_CALL_REJECTED if failed
				{				// so wait a bit to let it get ready
					Sleep(retryInterval);
					++retryCnt;
				}
				else
					retryCnt = retryCntMax;
			} while (retryCnt < retryCntMax);

			if (FAILED(hr))
				continue;

			long ProcessID;
			hr = Process->get_ProcessID(&ProcessID);

			if (SUCCEEDED(hr) && ProcessID == TargetPID)
			{
				Result = Process;
				break;
			}
		}

		return Result;
	}

	void AttachVS()
	{
		DWORD			TargetPID = GetCurrentProcessId();
		EnvDTE::Process* Process = FindVSProcess(TargetPID);
		if (Process)
			Process->Attach();
	}

	void DetachVS(bool waitForBreakOrEnd)
	{
		DWORD			TargetPID = GetCurrentProcessId();
		EnvDTE::Process* Process = FindVSProcess(TargetPID);
		if (Process)
			Process->Detach(variant_t(waitForBreakOrEnd));
	}

	bool FileIOisExist(const char* fileName)
	{

		DWORD dwAttrib = GetFileAttributesA(fileName);
		return (dwAttrib != INVALID_FILE_ATTRIBUTES) && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool FileIOcopy(const char* fromFile, const char* toFile, bool overwriteExisting)
	{
		return CopyFileA(fromFile, toFile, !overwriteExisting);
	}

	FILETIME	FileIOgetLastWriteTime(const char* fileName)
	{
		WIN32_FILE_ATTRIBUTE_DATA data;
		BOOL ok = GetFileAttributesExA(fileName, GetFileExInfoStandard, &data);
		return data.ftLastWriteTime;
	}

	bool FileIOisWriting(const char* fileName)
	{
		HANDLE h = CreateFileA(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h != INVALID_HANDLE_VALUE)
		{
			CloseHandle(h);
			return false;
		}
		return (GetLastError() == ERROR_SHARING_VIOLATION);
	}

	size_t CStrlastIndexOfChar(const char* str, char findChar)
	{
		intptr_t i = strlen(str) - 1;
		while (i >= 0)
		{
			if (str[i] == findChar)
				return i;
			--i;
		}
		return (size_t)-1;
	}

	// replace the last char of file name to '_'
	// which maintain the same file name length to avoid 
	// wrong offset in other area of the DLL
	bool patchFileName(char* fileName)
	{
		size_t	dotIdx = CStrlastIndexOfChar(fileName, '.');
		if (dotIdx != (size_t)-1)
		{
			fileName[dotIdx - 1] = '_';
			return true;
		}
		else
			return false;
	}

#define FREE_FILE(msg, retVal)			{							\
											free(fileContent);		\
											return retVal;			\
										}

#pragma warning( push )
#pragma warning( disable : 4200 )		// for flexible array member
	struct CV_INFO_PDB70
	{
		DWORD	CvSignature;
		GUID	Signature;
		DWORD	Age;
		BYTE	PdbFileName[];
	};
#pragma warning( pop )


	bool patchDLL(const char* dllPath, char patchedDllPath[MAX_PATH], char patchedPdbPath[MAX_PATH])
	{
		// init
		patchedDllPath[0] = '\0';
		patchedPdbPath[0] = '\0';

		// check DLL exist
		if (!FileIOisExist(dllPath))
		{
			RV_CORE_ASSERT(false, "Failed to load DLL file.");
			return false;
		}

		// create new DLL file Path
		strcpy(patchedDllPath, dllPath);
 		if (!patchFileName(patchedDllPath))
		{
			RV_CORE_ASSERT(false, "Failed to create new DLL file Path.");
			return false;
		}

		// open DLL and copy content to fileContent for easy parsing of the DLL content
		DWORD byteRead;
		HANDLE file = CreateFileA(dllPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			RV_CORE_ASSERT(false, "Failed to open DLL");
			return false;
		}
		size_t	fileSize = GetFileSize((HANDLE)file, NULL);
		BYTE* fileContent = (BYTE*)malloc(fileSize);
		bool	isFileReadOk = ReadFile((HANDLE)file, fileContent, (DWORD)fileSize, &byteRead, NULL);
		CloseHandle(file);
		if (!isFileReadOk || byteRead != fileSize)
		{
			FREE_FILE("Failed to read file.\n", false);
		}

		// check signature
		IMAGE_DOS_HEADER dosHeader = *(IMAGE_DOS_HEADER*)fileContent;
		if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
			FREE_FILE("Not IMAGE_DOS_SIGNATURE\n", false);

		// IMAGE_NT_HEADERS
		IMAGE_NT_HEADERS ntHeader = *((IMAGE_NT_HEADERS*)(fileContent + dosHeader.e_lfanew));
		if (ntHeader.Signature != IMAGE_NT_SIGNATURE)
			FREE_FILE("Not IMAGE_NT_SIGNATURE\n", false);

		IMAGE_DATA_DIRECTORY debugDir;
		if (ntHeader.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR_MAGIC && ntHeader.FileHeader.SizeOfOptionalHeader == sizeof(IMAGE_OPTIONAL_HEADER))
			debugDir = ntHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
		else
			FREE_FILE("Not IMAGE_NT_OPTIONAL_HDR_MAGIC\n", false);

		if (debugDir.VirtualAddress == 0 || debugDir.Size == 0)
			FREE_FILE("No IMAGE_DIRECTORY_ENTRY_DEBUG data\n", false);

		// find debug section
		int						debugDirSectionIdx = -1;
		IMAGE_SECTION_HEADER* allSectionHeaders = (IMAGE_SECTION_HEADER*)(fileContent + dosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS));
		for (int j = 0; j < ntHeader.FileHeader.NumberOfSections; ++j)
		{
			IMAGE_SECTION_HEADER sectionHeader = allSectionHeaders[j];
			if ((debugDir.VirtualAddress >= sectionHeader.VirtualAddress) &&
				(debugDir.VirtualAddress < sectionHeader.VirtualAddress + sectionHeader.Misc.VirtualSize))
			{
				debugDirSectionIdx = j;
				break;
			}
		}

		// read debug section
		char* pdbPath = nullptr;
		char	originalPdbPath[MAX_PATH];
		if (debugDirSectionIdx != -1)
		{
			// loop all debug directory
			int						numDebugDir = debugDir.Size / (int)sizeof(IMAGE_DEBUG_DIRECTORY);
			IMAGE_SECTION_HEADER	sectionHeader = allSectionHeaders[debugDirSectionIdx];
			IMAGE_DEBUG_DIRECTORY* allImageDebugDir = (IMAGE_DEBUG_DIRECTORY*)(fileContent + debugDir.VirtualAddress - (sectionHeader.VirtualAddress - sectionHeader.PointerToRawData));
			for (int i = 0; i < numDebugDir; ++i)
			{
				IMAGE_DEBUG_DIRECTORY imageDebugDir = allImageDebugDir[i];
				if (imageDebugDir.Type == IMAGE_DEBUG_TYPE_CODEVIEW)
				{
					DWORD signature = *((DWORD*)(fileContent + imageDebugDir.PointerToRawData));
					if (signature == 'SDSR')	// RSDS type, i.e. PDB70
					{
						CV_INFO_PDB70* debugInfo = ((CV_INFO_PDB70*)(fileContent + imageDebugDir.PointerToRawData));
						pdbPath = (char*)debugInfo->PdbFileName;
						strcpy(originalPdbPath, pdbPath);
						break;
					}
				}
			}
		}

		if (pdbPath == nullptr)
			FREE_FILE("No debug section is found.\n", false);

		// create new DLL and pdb
		patchFileName(pdbPath);
		if (FileIOisExist(originalPdbPath))
		{
			strcpy(patchedPdbPath, pdbPath);
			FileIOcopy(originalPdbPath, pdbPath, true);		// copy new PDB
		}
		HANDLE patchedDLL = CreateFileA(patchedDllPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD byteWrite;
		WriteFile(patchedDLL, fileContent, (DWORD)fileSize, &byteWrite, nullptr);	// generate patched DLL which points to the new PDB
		CloseHandle(patchedDLL);

		// clean up
		FREE_FILE("Patching DLL succeeded!!!.\n", true);
	}




	DLLdata loadDLL(const char* dllPath)
	{
		DLLdata data = { nullptr };

		// load the DLL
		data.dllHandle = LoadLibraryA(dllPath);

		if (dllPath == nullptr)
			return data;
		
		return data;
	}




	void ScriptEngine::OnDllUpdate()
	{
		// the DLL may not yet finish compliation and still writing to DLL
		if (!FileIOisWriting(m_DllPath))
		{
			if (IsDebuggerPresent())
				DetachVS(true);	// detach to release PDB lock

			// reload DLL
			Utilities::unloadDLL(m_Dll);
			m_Ok = patchDLL(m_DllPath, m_PatchedDllPath, m_PatchedPdbPath);
			if (m_Ok && (m_Dll = loadDLL(m_PatchedDllPath)).dllHandle)
			{
				if (m_IsStartWithDebugger)
					AttachVS();	// re-attach debugger

				m_DllLastWriteTime = m_DllTimeNew;
				printf("DLL reloaded.\n");
			}
			else
			{
				RV_ASSERT(false, "Failed to reload DLL");
			}
		}
	}

	ScriptEngine::ScriptEngine()
	{
		InitDll();
	}


	void ScriptEngine::OnUpdate()
	{
		//this needs to be called from inheritet script entity itself
		//m_IsTick = m_Dll.tick();
		//m_Dll.player();
	

		//script_obj->OnCreate();

		// check is DLL updated
		m_DllTimeNew = FileIOgetLastWriteTime(m_DllPath);
		if (!(m_DllLastWriteTime.dwHighDateTime == m_DllTimeNew.dwHighDateTime &&
			m_DllLastWriteTime.dwLowDateTime == m_DllTimeNew.dwLowDateTime))
		{
			OnDllUpdate();
		}
	}


	void ScriptEngine::ShutDown()
	{
		printf("shut down.\n");
		getchar();
		Utilities::unloadDLL(m_Dll);
		if (m_PatchedDllPath[0] != '\0')
			DeleteFileA(m_PatchedDllPath);
		if (m_PatchedDllPath[0] != '\0')
		{
			if (IsDebuggerPresent())
				DetachVS(true);	// may fail to delete the patchedPdbPath if a debugger 
			// is attached, MSVC locks the PDB file, 
			// so detach the debugger first.
			DeleteFileA(m_PatchedDllPath);
		}

		CoUninitialize();
	}

	using fn_t = ScriptEntity * (*)();

	ScriptEntity* ScriptEngine::GetScritpByName(std::string scriptName)
	{
		
		m_Dll.playerScript = (scripVoidPtr)GetProcAddress(m_Dll.dllHandle, scriptName.c_str());
	
		if (m_Dll.playerScript == nullptr)
		{
			RV_ENGINE_ERROR("Failed to read calss");
		}

		//
		fn_t new_obj_fn = (fn_t)m_Dll.playerScript;
		auto script_obj = new_obj_fn();
		return script_obj;
	}

	void ScriptEngine::InitDll()
	{
		m_IsStartWithDebugger = IsDebuggerPresent();
		RV_ENGINE_INFO("Start with debugger: {}", m_IsStartWithDebugger);

		CoInitialize(0);
		if (m_IsStartWithDebugger)
		{
			// re-attach the debugger to avoid killing the app process when stopping the debugger
			DetachVS(true);
			AttachVS();
		}

		GetModuleFileNameA(NULL, m_ExePath, MAX_PATH);
		const char* dllPath = "resourses/scripts/Native/MyRevoke-NativeScriptCore.dll";
		const char* loadDllPath = dllPath;

		strcpy(m_DllPath, dllPath);
		// create a copy of DLL and PDB

		m_Ok = patchDLL(m_DllPath, m_PatchedDllPath, m_PatchedPdbPath);
		if (m_Ok)
		{
			loadDllPath = m_PatchedDllPath;
			RV_ENGINE_INFO("Patch DLL succeeded");
		}
		else
		{
			RV_CORE_ASSERT(false, "Patch DLL failed\n");
			getchar();
			CoUninitialize();
			return;
		}

		// load DLL
		m_DllLastWriteTime = FileIOgetLastWriteTime(m_DllPath);
		m_Dll = loadDLL(loadDllPath);
		if (m_Dll.dllHandle == nullptr)
		{
			RV_CORE_ASSERT(false, "Failed to load DLL.");
			getchar();
			CoUninitialize();
			return;
		}

	}
}

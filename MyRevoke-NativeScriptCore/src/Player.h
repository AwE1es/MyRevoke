#pragma once


#include "MyRevoke/Scene/Components.h"

#include "MyRevoke/Scene/ScriptEntity.h"

#include "MyRevoke/Core/Input.h"
#include "MyRevoke/Utility/KeyCodes.h"

#include "MyRevoke/Core/Time.h"

using namespace Revoke;

extern "C" __declspec(dllexport) void Player();


class __declspec(dllexport) PlayerScript : public ScriptEntity
{
public:
	void OnCreate();
	
	void OnDestroy()
	{
	}

	void OnUpdate()
	{
	
		
	}
}; // TODO: in the object pannel you need to bind this script to the native script component, you need to somehow retrive this class to your main program from dll and bind PlayerScript to the chosen Entit

extern "C" __declspec(dllexport) ScriptEntity* GetPlayerScript();

 
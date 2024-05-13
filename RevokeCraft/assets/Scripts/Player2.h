#pragma once


#include "MyRevoke/Scene/Components.h"

#include "MyRevoke/Scene/ScriptEntity.h"

#include "MyRevoke/Core/Input.h"
#include "MyRevoke/Utility/KeyCodes.h"

#include "MyRevoke/Core/Time.h"

using namespace Revoke;

class PlayerScript2 : public ScriptEntity
{
public:
	PlayerScript2() = default;

	void OnCreate();
	
	void OnDestroy()
	{
	}

	void OnUpdate(Timestep ts);

private:
	int m_Count = 1;
	
}; 

extern "C" __declspec(dllexport) ScriptEntity * Player2();

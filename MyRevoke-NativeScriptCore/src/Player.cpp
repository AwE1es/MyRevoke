#include "Player.h"

#include <stdio.h>

#include <iostream>

ScriptEntity* Player()
{
	return new PlayerScript;
}


void PlayerScript::OnCreate()
{
	std::cout << " sdas\n";
}

void PlayerScript::OnUpdate(Timestep ts)
{

	//auto& comp = GetComponent<TransformComponent>();
	//comp.Position.x = comp.Position.x + 0.01f * ts;

}

#include "Player.h"

#include <stdio.h>

#include <iostream>

ScriptEntity* Player()
{
	return new PlayerScript;
}


void PlayerScript::OnCreate()
{
	std::cout << "dfasd";
}

void PlayerScript::OnUpdate(Timestep ts)
{
	auto& comp = GetComponent<TransformComponent>();
	comp.Position.x = comp.Position.x - 0.1f * ts;

}

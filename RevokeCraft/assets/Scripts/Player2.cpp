#include "Player2.h"

#include <stdio.h>

#include <iostream>

ScriptEntity* Player2()
{
	return new PlayerScript2;
}


void PlayerScript2::OnCreate()
{
	std::cout << "dfasd";
}

void PlayerScript2::OnUpdate(Timestep ts)
{
	auto& comp = GetComponent<TransformComponent>();
	comp.Position.x = comp.Position.x - 0.5f * ts;

}

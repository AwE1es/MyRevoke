#include "Player.h"

#include <stdio.h>

#include <iostream>

void Player()
{
	printf("player is \n");

}

ScriptEntity* GetPlayerScript()
{
	return new PlayerScript;
}

void PlayerScript::OnCreate()
{
	std::cout << " lolo\n";
}


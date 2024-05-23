#include "Player.h"

#include <iostream>
#include <memory>

#include <Box2D/Box2D.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>


#include "MyRevoke/Scene/Components.h"


ScriptEntity* Player()
{
	return new PlayerScript;
}


void PlayerScript::OnCreate()
{
	

}

void PlayerScript::OnUpdate(Timestep ts)
{
	
	auto& comp = GetComponent<TransformComponent>();
	
	comp.Position.x += 0.02f * ts;

}

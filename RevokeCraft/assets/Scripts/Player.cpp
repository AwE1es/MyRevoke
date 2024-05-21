#include "Player.h"

#include <iostream>
#include <memory>
//
//#include <Box2D/Box2D.h>
//#include <box2d/b2_world.h>
//#include <box2d/b2_polygon_shape.h>
//#include <box2d/b2_fixture.h>


#include "MyRevoke/Scene/Components.h"
#include "MyRevoke/Utility/KeyCodes.h"
#include "MyRevoke/Core/Input.h"

#include "MyRevoke/Core/Application.h"

ScriptEntity* Player()
{
	return new PlayerScript;
}


void PlayerScript::OnCreate()
{
	
	auto application = Application::Get();

}

void PlayerScript::OnUpdate(Timestep ts)
{

	//auto& comp = GetComponent<RigidBodyComponent>();
	//
	//b2Body* body = comp.Body;
	//
	//b2Vec2 velocity = body->GetLinearVelocity();
	//velocity.x = 5.0f; // Set constant horizontal speed
	//body->SetLinearVelocity(velocity);


	//if (false)
	//{
	//	
	//	std::cout << "Space Pressed \n";
	//
	//	b2Vec2 jumpvelocity = body->GetLinearVelocity();
	//	jumpvelocity.y = 0; // Reset vertical velocity
	//	body->SetLinearVelocity(velocity);
	//	body->ApplyLinearImpulseToCenter(b2Vec2(0, -10.0f), true); // Apply jump impulse
	//}
	
	//body->ApplyLinearImpulse({ 1.0f,1.0f }, { 1.0f, 1.0f }, false);


}

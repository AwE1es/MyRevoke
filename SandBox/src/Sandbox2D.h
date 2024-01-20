#include "MyRevoke.h"


#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" 
#include <glm/gtc/type_ptr.hpp> 


class Renderer2DTest : public Revoke::Layer
{
public:
	Renderer2DTest();
		
	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Revoke::Timestep deltaTime) override;
	void OnImGuiDraw() override;
	void OnEvent(Revoke::Event& e) override;
	

private:
};
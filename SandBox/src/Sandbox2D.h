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
	Revoke::Shared <Revoke::Texture> m_Texture2D;
	Revoke::OrthoCameraController m_CameraController2D;

	glm::vec4 m_MeshColor; 
	glm::vec2 m_MeshLocation; 
	glm::vec2 m_MeshScale; 

	float m_Framerate = 0.0f;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;
};
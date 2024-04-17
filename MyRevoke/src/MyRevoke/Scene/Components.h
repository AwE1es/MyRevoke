#pragma once
#include "SceneCamera.h"
#include "ScriptEntity.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MyRevoke/Renderer/Texture.h"

class b2Body;

namespace Revoke
{

	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name)
			:Name(name) {}
	};

	struct TransformComponent
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f , 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& pos)
			:Position(pos) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}

	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Shared<Texture> Texture2D;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color )
			:Color(color) {}

		SpriteRendererComponent(Shared<Texture> texture)
		: Texture2D(texture) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool isMain = true;

		bool FixedAspectRatio = false;
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void RecalculateProjection()
		{
			Camera.RecalculateProjection();
		}
	
		
	};

	struct BoxColisionComponent
	{
		BoxColisionComponent() = default;
		BoxColisionComponent(const BoxColisionComponent&) = default;

		glm::vec2 Size = { 0.5f, 0.5f };
		glm::vec2 Offset = { 0.0f, 0.0f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restriction = 0.0f;
		float ResitutionTreshhold = 0.5f;
		bool isSensor = false;

		//void* Fixture = nullptr;
	};

	struct RigidBodyComponent
	{
		enum class BodyType
		{
			StaticBody = 0,
			KinematicBody,
			DynamicBody
		};

		RigidBodyComponent() = default;
		RigidBodyComponent(const RigidBodyComponent&) = default;
		
		BodyType Type = BodyType::StaticBody;
		bool IsRotating = true;

		b2Body *Body = nullptr;
	};

	struct NativeScriptComponent //TODO: Move out with entt include
	{
		ScriptEntity* Instance = nullptr;

		ScriptEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}
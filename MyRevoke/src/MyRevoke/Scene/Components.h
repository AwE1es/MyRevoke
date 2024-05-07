#pragma once
#include "SceneCamera.h"
#include "ScriptEntity.h"
#include "MyRevoke/Renderer/Texture.h"

#include "MyRevoke/AudioManager/AudioSource.h"
#include "MyRevoke/AudioManager/AudioBuffer.h"
#include "MyRevoke/AudioManager/MusicBuffer.h"
#include "MyRevoke/Core/UniversallyUniqueIdentifiers.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

	struct IdComponent
	{
		UUID ID;

		IdComponent() = default;
		IdComponent(const IdComponent&) = default;
		IdComponent(const uint64_t id)
			:ID(id) {}

		void SetId(UUID id)
		{
			ID = id;
		}
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
		std::string Texture2D;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color )
			:Color(color) {}

		SpriteRendererComponent(std::string texture)
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

	struct SoundComponent
	{
		std::string AudioPath;

		uint32_t SoundID;
		uint32_t SourceID;


		float  Pitch = 1.0f;
		float  Gain = 1.0f;
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Velocity = { 0.0f, 0.0f, 0.0f };
		bool  LoopSound = false;
		
		bool HasPlayed = false;
	
		SoundComponent() = default;
		SoundComponent(const SoundComponent&) = default;

		SoundComponent(std::string audioPath)
		{
			SetPath(audioPath);
		}
		~SoundComponent()
		{
			ShutDown();
		}

		void SetPath(std::string audioPath)
		{
			AudioPath = audioPath;
			if (!AudioPath.empty())
			{
				SoundID = AudioBuffer::AddSound(AudioPath.c_str());
				SourceID = AudioSource::AddSound(Pitch, Gain, Position, Velocity, LoopSound, SoundID);
			}
		}
		void ShutDown()
		{
			AudioSource::ShutDown(SoundID);
			AudioBuffer::ShutDown();
		}
		//TODO: give control of destroing to the classes
	
	};
	struct SongComponent
	{
		std::string AudioPath;

		uint32_t SoundID;
		uint32_t SourceID;

		SongComponent() = default;
		SongComponent(const SongComponent&) = default;

		SongComponent(std::string audioPath)
		{
			SetPath(audioPath);
		}
		~SongComponent()
		{
			ShutDown();
		}

		void SetPath(std::string audioPath)
		{
			AudioPath = audioPath;
			if (!AudioPath.empty())
			{
				
			}
		}
		void ShutDown()
		{
			AudioSource::ShutDown(SoundID);
			AudioBuffer::ShutDown();
		}
	};
	

}
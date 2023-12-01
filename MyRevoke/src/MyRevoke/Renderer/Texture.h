#pragma once
#include <memory>

namespace Revoke
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot) const  = 0;
		virtual void UnBind() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		
	};

	class Texture2D : public Texture
	{
	public:
		static Shared<Texture2D> Create(const std::string& path);
		static Shared<Texture2D> Create(int width, int height);

	};
}
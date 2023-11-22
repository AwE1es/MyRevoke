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

	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(const char* path);

	};
}
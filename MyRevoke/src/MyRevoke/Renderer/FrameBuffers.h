#pragma once
#include "MyRevoke/Core/Core.h"

namespace Revoke
{
	struct FrameBufferStats
	{
		uint32_t Width, Height;

		uint32_t Samples = 1;
	};

	class FrameBuffers
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FrameBufferStats& GetSpecification() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		static Shared<FrameBuffers> Create(const FrameBufferStats& stats);
	};
}
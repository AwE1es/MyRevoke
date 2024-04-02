#pragma once
#include "MyRevoke/Core/Core.h"

namespace Revoke
{
	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA8,
		RED_INTEGER,
		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8

	};

	struct FrameBufferTextureSpec
	{
		FrameBufferTextureSpec() = default;
		FrameBufferTextureSpec(FramebufferTextureFormat format)
			: Format(format) {}
		FramebufferTextureFormat Format = FramebufferTextureFormat::None;
	};

	struct FrameBufferAttachmentSpec
	{
		FrameBufferAttachmentSpec() = default;
		FrameBufferAttachmentSpec(const std::initializer_list< FrameBufferTextureSpec> attachments)
			: Attachments(attachments) {}

		std::vector< FrameBufferTextureSpec> Attachments;
	};

	struct FrameBufferStats
	{
		uint32_t Width = 0 , Height = 0;
		FrameBufferAttachmentSpec Attachments;

		uint32_t Samples = 1;
	};

	class FrameBuffers
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FrameBufferStats& GetSpecification() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearColorTextureAttachment(uint32_t attachmentIndex, int value) = 0;

		static Shared<FrameBuffers> Create(const FrameBufferStats& stats);
	};
}
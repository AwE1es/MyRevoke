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
		FrameBuffers(const FrameBufferStats& stats);
		~FrameBuffers();

		void Bind();
		void UnBind();

		void Create();

		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const;

		const FrameBufferStats& GetSpecification() const;
		int ReadPixel(uint32_t attachmentIndex, int x, int y);

		void Resize(uint32_t width, uint32_t height);

		void ClearColorTextureAttachment(uint32_t attachmentIndex, int value);

	private:
		FrameBufferStats m_Stats;
		uint32_t m_RendererID = 0;

		uint32_t m_DepthAttachment = 0;
		std::vector<FrameBufferTextureSpec> m_ColorAttachmentSpecs;
		FrameBufferTextureSpec m_DepthAttachmentSpec = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_RendererIDs;
	};


}
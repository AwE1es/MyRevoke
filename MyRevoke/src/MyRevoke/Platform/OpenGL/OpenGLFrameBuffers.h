#pragma once
#include "MyRevoke/Renderer/FrameBuffers.h"

namespace Revoke
{
	class OpenGLFrameBuffers : public FrameBuffers
	{
	public:
		OpenGLFrameBuffers(const FrameBufferStats& stats);
		~OpenGLFrameBuffers();

		virtual void Bind() override;
		virtual void UnBind() override;

		void Create();

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;

		virtual const FrameBufferStats& GetSpecification() const override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void ClearColorTextureAttachment(uint32_t attachmentIndex, int value) override;

	private:
		FrameBufferStats m_Stats;
		uint32_t m_RendererID = 0;

		uint32_t m_DepthAttachment = 0;
		std::vector<FrameBufferTextureSpec> m_ColorAttachmentSpecs;
		FrameBufferTextureSpec m_DepthAttachmentSpec = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_RendererIDs;
	};
}
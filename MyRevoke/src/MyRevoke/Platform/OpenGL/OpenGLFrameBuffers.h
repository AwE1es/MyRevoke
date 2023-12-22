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

		virtual uint32_t GetColorAttachmentRendererID() const override;

		virtual const FrameBufferStats& GetSpecification() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

	private:
		FrameBufferStats m_Stats;
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
	};
}
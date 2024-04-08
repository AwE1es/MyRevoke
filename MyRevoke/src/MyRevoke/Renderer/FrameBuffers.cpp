#include "rvpch.h"
#include "FrameBuffers.h"


#include <glad/glad.h>


namespace Revoke
{
	
    static const uint32_t s_MaxFramebufferSize = 8192;

    static GLenum TextureTarget(bool multisampled)
    {
        return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void CreateTexture(bool multisampled, uint32_t* outID, uint32_t count)
    {
        glCreateTextures(TextureTarget(multisampled), count, outID);
    }
    static void BindTexture(bool multisampled, uint32_t ID)
    {
        glBindTexture(TextureTarget(multisampled), ID);
    }
    static bool IsDepthFormat(FramebufferTextureFormat format)
    {
        switch (format)
        {
        case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
        }
        return false;
    }
    static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
    {
        bool multisampled = samples > 1;

        if (multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
        }
        else
        {
            glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
    }
    static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
    {
        bool multisampled = samples > 1;

        if (multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
    }

    static GLenum ToGLTextureFormat(FramebufferTextureFormat format)
    {
        switch (format)
        {

        case Revoke::FramebufferTextureFormat::RGBA8: return GL_RGBA8;
            break;
        case Revoke::FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
            break;
        }
    }


    FrameBuffers::FrameBuffers(const FrameBufferStats& stats)
        : m_Stats(stats)
    {
        for (auto spec : m_Stats.Attachments.Attachments)
        {
            if (!IsDepthFormat(spec.Format))
                m_ColorAttachmentSpecs.emplace_back(spec);
            else
            {
                m_DepthAttachmentSpec = spec;
            }
        }
        Create();
    }

    FrameBuffers::~FrameBuffers()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(m_RendererIDs.size(), m_RendererIDs.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void FrameBuffers::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Stats.Width, m_Stats.Height);


    }

    void FrameBuffers::UnBind()
    {

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffers::Create()
    {

        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(m_RendererIDs.size(), m_RendererIDs.data());
            glDeleteTextures(1, &m_DepthAttachment);

            m_RendererIDs.clear();
            m_DepthAttachment = 0;
        }

        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        bool multisample = m_Stats.Samples > 1;

        if (m_ColorAttachmentSpecs.size())
        {
            m_RendererIDs.resize(m_ColorAttachmentSpecs.size());
            CreateTexture(multisample, m_RendererIDs.data(), m_RendererIDs.size());

            for (int i = 0; i < m_RendererIDs.size(); i++)
            {
                BindTexture(multisample, m_RendererIDs[i]);

                switch (m_ColorAttachmentSpecs[i].Format)
                {
                case FramebufferTextureFormat::RGBA8:
                {
                    AttachColorTexture(m_RendererIDs[i], m_Stats.Samples, GL_RGBA8, GL_RGBA, m_Stats.Width, m_Stats.Height, i);
                    break;
                }
                case FramebufferTextureFormat::RED_INTEGER:
                {
                    AttachColorTexture(m_RendererIDs[i], m_Stats.Samples, GL_R32I, GL_RED_INTEGER, m_Stats.Width, m_Stats.Height, i);
                    break;
                }
                }

            }
        }

        if (m_DepthAttachmentSpec.Format != FramebufferTextureFormat::None)
        {
            CreateTexture(multisample, &m_DepthAttachment, 1);
            BindTexture(multisample, m_DepthAttachment);
            switch (m_DepthAttachmentSpec.Format)
            {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                AttachDepthTexture(m_DepthAttachment, m_Stats.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, m_Stats.Width, m_Stats.Height);
                break;
            }
        }

        if (m_RendererIDs.size() > 1)
        {
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 ,GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(m_RendererIDs.size(), buffers);
        }
        else if (m_RendererIDs.empty())
        {
            glDrawBuffer(GL_NONE);
        }

        RV_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t FrameBuffers::GetColorAttachmentRendererID(uint32_t index) const
    {
        return m_RendererIDs[index];
    }
    const FrameBufferStats& FrameBuffers::GetSpecification() const
    {
        return m_Stats;
    }
    int FrameBuffers::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }
    void FrameBuffers::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
        {
            RV_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
            return;
        }
        m_Stats.Width = width;
        m_Stats.Height = height;

        Create();
    }

    void FrameBuffers::ClearColorTextureAttachment(uint32_t attachmentIndex, int value)
    {
        auto& spec = m_ColorAttachmentSpecs[attachmentIndex];
        glClearTexImage(m_RendererIDs[attachmentIndex], 0, ToGLTextureFormat(spec.Format), GL_INT, &value);
    }

};

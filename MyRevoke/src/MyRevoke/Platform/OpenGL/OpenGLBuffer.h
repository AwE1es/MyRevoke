#pragma once

#include "MyRevoke/Renderer/Buffer.h"

namespace Revoke
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
		~OpenGLVertexBuffer();
						
		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetLayout(const BufferLayout& layout) override;
		virtual void InitData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual inline uint32_t GetCount() const override { return m_Count; }


	private:
		uint32_t m_RendererID, m_Count;
	};

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		
		virtual void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuff)  override;
		virtual void SetIndexBuffer(const Shared<IndexBuffer>& indexBuff)  override;

		virtual const std::vector<Shared<VertexBuffer>>& GetVertexBuffer() const override {	return m_VertexBuff;}
		virtual const Shared <IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuff; }
	private:
		std::vector <Shared<VertexBuffer>> m_VertexBuff;
		uint32_t m_VertexBufferIndexOffset = 0;
		Shared <IndexBuffer> m_IndexBuff;
		uint32_t m_RendererID;
	};
}
#pragma once

#include "MyRevoke/Renderer/Buffer.h"

namespace Revoke
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer();
						
		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetLayout(const BufferLayout& layout) override;
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

	class OpenGLVertexArrey : public VertexArray
	{
	public:
		OpenGLVertexArrey();
		virtual ~OpenGLVertexArrey();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuff)  override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuff)  override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffe() const override {	return m_VertexBuff;}
		virtual const std::shared_ptr <IndexBuffer>& GetIndexBuffe() const override { return m_IndexBuff; }
	private:
		std::vector <std::shared_ptr <VertexBuffer>> m_VertexBuff;
		std::shared_ptr <IndexBuffer> m_IndexBuff;
		uint32_t m_RendererID;
	};
}
#include "rvpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Revoke {
	//Temp
	static GLenum ShaderDataTypeToOpenGLDataType(ShaderDataTypes type)
	{
		switch (type)
		{
		case Revoke::ShaderDataTypes::Float:	return GL_FLOAT;
		case Revoke::ShaderDataTypes::Float2:	return GL_FLOAT;
		case Revoke::ShaderDataTypes::Float3:	return GL_FLOAT;
		case Revoke::ShaderDataTypes::Float4:	return GL_FLOAT;
		case Revoke::ShaderDataTypes::Mat3:		return GL_FLOAT;
		case Revoke::ShaderDataTypes::Mat4:		return GL_FLOAT;
		case Revoke::ShaderDataTypes::Int:		return GL_INT;
		case Revoke::ShaderDataTypes::Int2:		return GL_INT;
		case Revoke::ShaderDataTypes::Int3:		return GL_INT;
		case Revoke::ShaderDataTypes::Int4:		return GL_INT;
		case Revoke::ShaderDataTypes::Bool:		return GL_BOOL;
		default:
			RV_CORE_ASSERT(false, "Unknown Shader Data Type");
			return 0;
		}
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0	);
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
	}

	void OpenGLVertexBuffer::InitData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	//***********************************************************************************
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}
	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLIndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	

	//**********************************************************************************

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Shared<VertexBuffer>& vertexBuff)
	{
		glBindVertexArray(m_RendererID);
		vertexBuff->Bind();

		const auto& layout = vertexBuff->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataTypes::Float:
			case ShaderDataTypes::Float2:
			case ShaderDataTypes::Float3:
			case ShaderDataTypes::Float4:
			{
				glEnableVertexAttribArray(m_VertexBufferIndexOffset);
				glVertexAttribPointer(m_VertexBufferIndexOffset,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLDataType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndexOffset++;
				break;
			}
			case ShaderDataTypes::Int:
			case ShaderDataTypes::Int2:
			case ShaderDataTypes::Int3:
			case ShaderDataTypes::Int4:
			case ShaderDataTypes::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndexOffset);
				glVertexAttribIPointer(m_VertexBufferIndexOffset,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLDataType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndexOffset++;
				break;
			}
			case ShaderDataTypes::Mat3:
			case ShaderDataTypes::Mat4:
			
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndexOffset);
					glVertexAttribPointer(m_VertexBufferIndexOffset,
						count,
						ShaderDataTypeToOpenGLDataType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndexOffset, 1);
					m_VertexBufferIndexOffset++;
				}
				break;
			}
			default:
				RV_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		m_VertexBuff.push_back(vertexBuff);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Shared<IndexBuffer>& indexBuff)
	{
		glBindVertexArray(m_RendererID);
		indexBuff->Bind();
		
		m_IndexBuff = indexBuff;
	}



}

#pragma once

namespace Revoke
{
	enum class ShaderDataTypes
	{
		None = 0,
		Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};
	
	static uint32_t ShaderDataTypeSize(ShaderDataTypes type)
	{
		switch (type)
		{	
		case Revoke::ShaderDataTypes::Float:		return 4;
		case Revoke::ShaderDataTypes::Float2:		return 4 * 2;
		case Revoke::ShaderDataTypes::Float3:		return 4 * 3;
		case Revoke::ShaderDataTypes::Float4:		return 4 * 4;
		case Revoke::ShaderDataTypes::Mat3:			return 4 * 3 * 3;
		case Revoke::ShaderDataTypes::Mat4:			return 4 * 4 * 4;
		case Revoke::ShaderDataTypes::Int:			return 4;
		case Revoke::ShaderDataTypes::Int2:			return 4 * 2;
		case Revoke::ShaderDataTypes::Int3:			return 4 * 3;
		case Revoke::ShaderDataTypes::Int4:			return 4 * 4;
		case Revoke::ShaderDataTypes::Bool:			return 1;
		default:
			RV_CORE_ASSERT(false, "Unknown Shader Data Type");
			return 0;
		}
	}

	struct BufferElements
	{
		BufferElements(ShaderDataTypes type, std::string name, bool normalized = false)
			: Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{

		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case Revoke::ShaderDataTypes::Float:	return 1;				
			case Revoke::ShaderDataTypes::Float2:	return 2;			
			case Revoke::ShaderDataTypes::Float3:	return 3;				
			case Revoke::ShaderDataTypes::Float4:	return 4;					
			case Revoke::ShaderDataTypes::Mat3:		return 3 * 3;				
			case Revoke::ShaderDataTypes::Mat4:		return 4 * 4;	
			case Revoke::ShaderDataTypes::Int:		return 1;	
			case Revoke::ShaderDataTypes::Int2:		return 2;		
			case Revoke::ShaderDataTypes::Int3:		return 3;			
			case Revoke::ShaderDataTypes::Int4:		return 4;
			case Revoke::ShaderDataTypes::Bool:		return 1;				
			default:
				RV_CORE_ASSERT(false, "Unknown Shader Data Type");
				return 0;
			}
		}

		std::string Name;
		int Size;
		int Offset;
		ShaderDataTypes Type;
		bool Normalized;
	};

	class  BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElements>& elements)
			: m_Elements(elements)
		{
			CalculateStrideAndOffset();
		}

		inline const std::vector<BufferElements>& GetElements() const { return m_Elements; }

		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElements>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElements>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElements>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElements>::const_iterator end() const { return m_Elements.end(); }
	private:

		void CalculateStrideAndOffset()
		{
			m_Stride = 0;
			uint32_t offset = 0;
			for (auto& element: m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

		std::vector <BufferElements> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual  ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual  ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	};

	class VertexArray
	{
	public:
		virtual  ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuff)  = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuff)  = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}
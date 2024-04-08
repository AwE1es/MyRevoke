#pragma once
#include <memory>

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
		VertexBuffer(float* vertices, uint32_t size);
		VertexBuffer(uint32_t size);
		~VertexBuffer();

		void Bind() const;
		void UnBind() const ;

		void SetLayout(const BufferLayout& layout) ;
		void InitData(const void* data, uint32_t size) ;

		const BufferLayout& GetLayout() const  { return m_Layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class IndexBuffer 
	{
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		void UnBind() const;

		uint32_t GetCount() const  { return m_Count; }


	private:
		uint32_t m_RendererID, m_Count;
	};

	class VertexArray 
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void UnBind() const;

		void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuff);
		void SetIndexBuffer(const Shared<IndexBuffer>& indexBuff);

		const std::vector<Shared<VertexBuffer>>& GetVertexBuffer() const { return m_VertexBuff; }
		const Shared <IndexBuffer>& GetIndexBuffer() const { return m_IndexBuff; }
	private:
		std::vector <Shared<VertexBuffer>> m_VertexBuff;
		uint32_t m_VertexBufferIndexOffset = 0;
		Shared <IndexBuffer> m_IndexBuff;
		uint32_t m_RendererID;
	};

}
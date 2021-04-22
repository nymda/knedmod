#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace td {

	struct Vec2 {
		float x;
		float y;
	};

	struct Vec3 {
		float x;
		float y;
		float z;
	};

	struct Vec4 {
		float x;
		float y;
		float z;
		float w;
	};

	struct Color {
		float r;
		float g;
		float b;
		float a;
	};

	struct particleInfo {
		float R;
		float G;
		float B;
		float A;
		float U1;
		float U2;
		float U3;
		float U4;
	};

	namespace {
		template <typename T>
		struct Vector2 {
			T m_X; T m_Y;
		};

		template <typename T>
		struct Vector3 {
			T m_X; T m_Y; T m_Z;
		};

		template <typename T>
		struct Vector4 {
			T m_X; T m_Y; T m_Z; T m_W;
		};
	}

	using Vector3_u8 = Vector3<uint8_t>;
	using Vector3_u16 = Vector3<uint16_t>;

	using Vector2_i32 = Vector2<int32_t>;
	using Vector3_i32 = Vector3<int32_t>;
	using Vector4_i32 = Vector4<int32_t>;

	using Vector2_u32 = Vector2<uint32_t>;
	using Vector3_u32 = Vector3<uint32_t>;

	using Vector2_f32 = Vector2<float>;
	using Vector3_f32 = Vector3<float>;
	using Vector4_f32 = Vector4<float>;

	struct Matrix4x4 {
		float m[4][4];
	};

	struct Transform
	{
		Vector3_f32 m_Position;
		Vector4_f32 m_Rotation;
	};

	class small_string
	{
	public:
		small_string() {};
		small_string(const char* str) {
			size_t len = strlen(str);
			char* dst = m_StackBuffer[15] ? m_HeapBuffer : &m_StackBuffer[0];

			if (len > 15)
			{
				dst = (char*)malloc(len + 1);

				if (dst == nullptr)
				{
					return;
				}

				if (m_StackBuffer[15])
				{
					free(m_HeapBuffer);
				}
				else
				{
					m_StackBuffer[15] = 1;
				}

				m_HeapBuffer = dst;
			}

			memcpy(dst, str, len);
			dst[len] = 0;
		}
		~small_string() {
			if (m_StackBuffer[15])
			{
				free(m_HeapBuffer);
			}
		}

		const char* c_str() const { return m_StackBuffer[15] ? m_HeapBuffer : &m_StackBuffer[0]; }

	private:
		union {
			char* m_HeapBuffer;
			char m_StackBuffer[16] = { 0 };
		};
	};

	//template<class T>
	//T* MemoryNew()
	//{
	//	return static_cast<T*>(MemoryAlloc(sizeof(T)));
	//}

	//template<class T>
	//T* MemoryNew(size_t size)
	//{
	//	return static_cast<T*>(MemoryAlloc(sizeof(T) * size));
	//}

	//template<typename T>
	//class small_vector {
	//public:
	//	small_vector() {};
	//	small_vector(uint32_t capacity) { reserve(capacity); };
	//	small_vector(const small_vector&) = delete;
	//	void operator=(const small_vector&) = delete;
	//	~small_vector() { clear(); }

	//	T* begin() const { return m_Data; }
	//	T* end() const { return m_Data + m_Size; }
	//	T* data() const { return m_Data; }
	//	uint32_t size() const { return m_Size; }
	//	uint32_t capacity() const { return m_Capacity; }

	//	void push_back(const T& value)
	//	{
	//		if (m_Capacity <= m_Size)
	//		{
	//			reserve(2 * m_Capacity + 1);
	//		}

	//		m_Data[m_Size] = value;
	//		++m_Size;
	//	}

	//	void reserve(uint32_t capacity) {
	//		if (auto memory = MemoryNew<T>(capacity))
	//		{
	//			T* data = m_Data;
	//			uint32_t size = m_Size;

	//			if (data != nullptr)
	//			{
	//				if (size >= capacity)
	//				{
	//					size = capacity;
	//				}
	//				else
	//				{
	//					memset(memory, sizeof(T) * size, sizeof(T) * capacity - size);
	//				}

	//				memcpy(m_Data, data, sizeof(T) * size);
	//				MemoryFree(data);
	//				m_Size = size;
	//			}
	//			else
	//			{
	//				m_Size = 0;
	//				memset(memory, 0, sizeof(T) * capacity);
	//			}

	//			m_Data = memory;
	//			m_Capacity = capacity;
	//		}
	//	}

	//	void clear()
	//	{
	//		if (m_Data != nullptr)
	//		{
	//			m_Size = 0;
	//			m_Capacity = 0;
	//			MemoryFree(m_Data);
	//			m_Data = nullptr;
	//		}
	//	}

	//private:
	//	uint32_t m_Size = 0;
	//	uint32_t m_Capacity = 0;
	//	T* m_Data = nullptr;
	//};

	template<typename T>
	class small_vector {
	public:
		small_vector() {};
		small_vector(uint32_t capacaity) { reserve(capacity); };
		~small_vector() { clear(); }

		T** begin() const { return m_Data; }
		T** end() const { return m_Data + m_Size; }
		T** data() const { return m_Data; }
		uint32_t size() const { return m_Size; }
		uint32_t capacity() const { return m_Capacity; }

		void reserve(uint32_t capacity) {
			T** data = m_Data;
			uint32_t size = m_Size;

			if (auto memory = malloc(sizeof(T) * capacity))
			{
				m_Size = 0;
				m_Capacity = capacity;
				m_Data = (T**)memory;
				memset(m_Data, 0, sizeof(T) * capacity);

				if (data)
				{
					if (size > m_Capacity)
					{
						size = m_Capacity;
					}

					m_Size = capacity;
					memcpy(m_Data, data, sizeof(T) * size);
					free(data);
				}
			}
		}

		void clear()
		{
			if (m_Data)
			{
				m_Size = 0;
				m_Capacity = 0;
				free(m_Data);
				m_Data = nullptr;
			}
		}

	private:
		uint32_t m_Size = 0;
		uint32_t m_Capacity = 0;
		T** m_Data = nullptr;
	};

	/*
	 * Represents a known good function signature.
	 *
	 * Example usage:
	 * decltype(Teardown::Game::Update)::Type Game_Update = FindSignature(Teardown::Game.Signature);
	 *
	 * FindSignature is a user defined function accepting a const* char which scans the main Teardown.exe module.
	 */
	template<typename T>
	struct function_signature
	{
		/// You can retrieve the function type using decltype(Teardown::Game::Update)::Type.
		using Type = T;
		/// You can retrieve the function sigature using Teardown::Game::Update.Signature. Signatures use "\x2A" to represent wild cards.
		const char* Signature = "";
	};

	static_assert(sizeof(small_string) == 16, "small_string size is incorrect!");
	static_assert(sizeof(small_vector<void>) == 16, "small_vector size is incorrect!");


};

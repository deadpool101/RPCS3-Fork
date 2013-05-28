#pragma once

template<typename T> class Array
{
	u32 m_count;
	T* m_array;

public:
	Array()
		: m_count(0)
		, m_array(NULL)
	{
	}

	~Array()
	{
		Clear();
	}

	inline bool RemoveAt(const u32 from, const u32 count = 1)
	{
		if(!GetCount()) return false;
		const u32 to = from + count;
		if(to > GetCount()) return false;
		
		for(u32 i=0; i<count; ++i) m_array[from + i].~T();
		memmove(m_array + from, m_array + to, (m_count-to) * sizeof(T));
		m_count -= count;
		
		return true;
	}
	
	void InsertRoomEnd(const u32 size)
	{
		_InsertRoomEnd(size);
	}
	
	bool InsertRoom(const u32 pos, const u32 size)
	{
		if(pos >= m_count) return false;
		
		_InsertRoomEnd(size);
		memmove(m_array + pos + size, m_array + pos, sizeof(T) * (m_count - size - pos));

		return true;
	}

	inline bool Move(const u32 pos, T* data)
	{
		if(!InsertRoom(pos, 1)) return false;
		
		memcpy(m_array + pos, data, sizeof(T));
		free(data);
		
		return true;
	}

	inline u32 Move(T* data)
	{
		_InsertRoomEnd(1);

		memcpy(m_array + GetCount() - 1, data, sizeof(T));
		free(data);

		return m_count - 1;
	}

	inline bool Add(const u32 pos, T*& data)
	{
		if(!InsertRoom(pos, 1)) return false;
		
		memcpy(m_array + pos, data, sizeof(T));
		free(data);
		data = m_array + pos;
		
		return true;
	}

	inline u32 Add(T*& data)
	{
		_InsertRoomEnd(1);

		memcpy(m_array + GetCount() - 1, data, sizeof(T));
		free(data);
		data = m_array + GetCount() - 1;
		
		return m_count - 1;
	}

	inline bool AddCpy(const u32 pos, const T* data, u64 count = 1)
	{
		if(!InsertRoom(pos, count)) return false;
		
		memcpy(m_array + pos, data, sizeof(T) * count);

		return true;
	}

	inline bool AddCpy(const u32 pos, const T& data)
	{
		return AddCpy(pos, &data);
	}

	inline u32 AddCpy(const T* data, u64 count = 1)
	{
		_InsertRoomEnd(count);

		memcpy(m_array + m_count - count, data, sizeof(T)*count);

		return m_count - count;
	}

	inline u32 AddCpy(const T& data)
	{
		return AddCpy(&data);
	}

	inline void Clear()
	{
		u32 count = m_count;
		m_count = 0;
		for(u32 i=0; i<count; ++i) m_array[i].~T();
		safe_delete(m_array);
	}

	inline T& Get(u32 num)
	{
		//if(num >= GetCount()) return *new T();
		return m_array[num];
	}

	u32 GetCount() const { return m_count; }

	void SetCount(const u32 count, bool memzero = true)
	{
		if(GetCount() >= count) return;
		
		_InsertRoomEnd(count - GetCount());

		if(memzero) memset(m_array + GetCount(), 0, count - GetCount());
	}

	void Reserve(const u32 count)
	{
		SetCount(GetCount() + count);
	}

	void AppendFrom(const Array<T>& src)
	{
		if(!src.GetCount()) return;

		Reserve(src.GetCount());

		memcpy(m_array, &src[0], GetCount() * sizeof(T));
	}

	void CopyFrom(const Array<T>& src)
	{
		Clear();

		AppendFrom(src);
	}

	inline T* GetPtr() { return m_array; }

	T& operator[](u32 num) const { return m_array[num]; }
	
	T* operator + (u32 right) const
	{
		return m_array + right;
	}

	T* operator ->()
	{
		return m_array;
	}

protected:	
	void _InsertRoomEnd(const u32 size)
	{
		if(!size) return;

		m_array = m_count ? (T*)realloc(m_array, sizeof(T) * (m_count + size)) : (T*)malloc(sizeof(T) * size);
		m_count += size;
	}
};

template<typename T> struct Stack : public Array<T>
{
	Stack() : Array<T>()
	{
	}

	~Stack()
	{
		Clear();
	}

	void Push(const T data) { AddCpy(data); }

	T Pop()
	{
		const u32 pos = GetCount() - 1;

		const T ret = Get(pos);
		RemoveAt(pos);

		return ret;
	}
};

template<typename T> class ArrayF
{
	u32 m_count;
	T** m_array;

public:
	ArrayF()
		: m_count(0)
		, m_array(NULL)
	{
	}

	virtual ~ArrayF()
	{
		Clear();
	}

	inline bool RemoveFAt(const u32 from, const u32 count = 1)
	{
		if(from + count > m_count) return false;

		memmove(&m_array[from], &m_array[from+count], (m_count-(from+count)) * sizeof(T**));

		m_count -= count;
		return true;
	}

	inline bool RemoveAt(const u32 from, const u32 count = 1)
	{
		if(from + count > m_count) return false;

		for(uint i = from; i < from + count; ++i)
		{
			free(m_array[i]);
		}

		return RemoveFAt(from, count);
	}

	inline u32 Add(T& data)
	{
		return Add(&data);
	}

	inline u32 Add(T* data)
	{
		if(!m_array)
		{
			m_array = (T**)malloc(sizeof(T*));
		}
		else
		{
			m_array = (T**)realloc(m_array, sizeof(T*) * (m_count + 1));
		}

		m_array[m_count] = data;
		return m_count++;
	}

	inline void ClearF()
	{
		if(m_count == 0) return;

		m_count = 0;
		m_array = NULL;
	}

	inline void Clear()
	{
		if(m_count == 0) return;

		m_count = 0;
		safe_delete(m_array);
	}

	inline T& Get(const u64 num)
	{
		//if(m_count <= num) *m_array[0]; //TODO
		return *m_array[num];
	}

	T** operator + (u32 right) const
	{
		return m_array + right;
	}

	T* operator ->()
	{
		return *m_array;
	}

	inline T** GetPtr()
	{
		return m_array;
	}

	inline u32 GetCount() const { return m_count; }
	T& operator[](u32 num) const { return *m_array[num]; }
};

template<typename T> struct ScopedPtr
{
private:
	T* m_ptr;

public:
	ScopedPtr() : m_ptr(nullptr)
	{
	}

	ScopedPtr(T* ptr) : m_ptr(ptr)
	{
	}

	~ScopedPtr()
	{
		Swap(nullptr);
	}

	operator T*() { return m_ptr; }
	operator const T*() const { return m_ptr; }

	T* operator ->() { return m_ptr; }
	const T* operator ->() const { return m_ptr; }

	void Swap(T* ptr)
	{
		delete m_ptr;
		m_ptr = ptr;
	}
};
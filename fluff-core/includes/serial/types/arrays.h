#pragma once

#include <common.h>
#include <serial/archives/iarchive.h>

namespace fluff { namespace serial {

	class FLUFF_API IArray
	{
	protected:
		size_t Count_;
		size_t NameLength_;
		char * Name_;
	public:
		virtual void ** GetDataPtr() = 0;
		virtual const size_t GetTypeSize() const = 0;
		virtual const char   GetTypeHandle() const = 0;

		char * const GetName() const;
		const size_t GetNameLength() const;
		const size_t GetDataCount() const;
	};

	template <typename Type>
	class Array : public IArray
	{
	protected:
		void * Data_;
	public:
		Array(const char * Name, Type * Data, size_t Count)
		{
			NameLength_ = strlen(Name) + 1;
			Name_ = static_cast<char*>(calloc(NameLength_, sizeof(char)));
			strcpy_s(Name_, NameLength_, Name);

			Data_ = calloc(Count, sizeof(Type));
			memcpy(Data_, Data, sizeof(Type) * Count);

			Count_ = Count;
		}

		~Array()
		{
			free(Name_);
			free(Data_);
		}

		void ** GetDataPtr() override
		{
			return &Data_;
		}

		virtual const size_t GetTypeSize() const = 0;
	};

	class FLUFF_API CharArray : public Array<char>
	{
		static size_t Size_;
	public:
		CharArray(const char * Name, char * Value, size_t Count);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API UCharArray : public Array<unsigned char>
	{
		static size_t Size_;
	public:
		UCharArray(const char * Name, unsigned char * Value, size_t Count);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API ShortArray : public Array<short>
	{
		static size_t Size_;
	public:
		ShortArray(const char * Name, short * Value, size_t Count);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API UShortArray : public Array<unsigned short>
	{
		static size_t Size_;
	public:
		UShortArray(const char * Name, unsigned short * Value, size_t Count);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API IntArray : public Array<int>
	{
		static size_t Size_;
	public:
		IntArray(const char * Name, int * Value, size_t Count);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API UIntArray : public Array<unsigned int>
	{
		static size_t Size_;
	public:
		UIntArray(const char * Name, unsigned int * Value, size_t Count);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API LongArray : public Array<long long>
	{
		static size_t Size_;
	public:
		LongArray(const char * Name, long long * Value, size_t Count);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API ULongArray : public Array<unsigned long long>
	{
		static size_t Size_;
	public:
		ULongArray(const char * Name, unsigned long long * Value, size_t Count);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API FloatArray : public Array<float>
	{
		static size_t Size_;
	public:
		FloatArray(const char * Name, float * Value, size_t Count);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API DoubleArray : public Array<double>
	{
		static size_t Size_;
	public:
		DoubleArray(const char * Name, double * Value, size_t Count);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	void FLUFF_API Write(IArchive * Archive, IArray * Ar);
	void FLUFF_API Read(IArchive * Archive, IArray ** Ar, size_t * Pointer);

} }
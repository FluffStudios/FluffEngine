#pragma once

#include <stdlib.h>
#include <string.h>

#include <common.h>
#include <serial/archives/iarchive.h>

namespace fluff { namespace serial {

	class IPrimitive
	{
	public:
		virtual void * GetDataPtr() = 0;
		virtual char * const GetName() const = 0;
		virtual size_t GetNameLength() const = 0;
		virtual const size_t GetTypeSize() const = 0;
		virtual const char   GetTypeHandle() const = 0;
	};

	template <typename Type>
	class Primitive : public IPrimitive
	{
	protected:
		char * Name_;
		size_t NameLength_;
		Type Value_;
	public:
		Primitive(const char * Name, Type Value)
		{
			NameLength_ = strlen(Name) + 1;
			Name_ = static_cast<char*>(calloc(NameLength_, sizeof(char)));
			strcpy_s(Name_, NameLength_, Name);
			memcpy(&Value_, &Value, sizeof(Type));
		}

		~Primitive()
		{
			free(Name_);
		}

		char * const GetName() const
		{
			return Name_;
		}

		size_t GetNameLength() const
		{
			return NameLength_;
		}

		void * GetDataPtr()
		{
			return const_cast<Type*>(&Value_);
		}

		virtual const size_t GetTypeSize() const = 0;
	};

	class FLUFF_API CharPrimitive : public Primitive<char>
	{
		static size_t Size_;
	public:
		CharPrimitive(const char * Name, char Value);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API UCharPrimitive : public Primitive<unsigned char>
	{
		static size_t Size_;
	public:
		UCharPrimitive(const char * Name, unsigned char Value);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API ShortPrimitive : public Primitive<short>
	{
		static size_t Size_;
	public:
		ShortPrimitive(const char * Name, short Value);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API UShortPrimitive : public Primitive<unsigned short>
	{
		static size_t Size_;
	public:
		UShortPrimitive(const char * Name, unsigned short Value);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API IntPrimitive : public Primitive<int>
	{
		static size_t Size_;
	public:
		IntPrimitive(const char * Name, int Value);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API UIntPrimitive : public Primitive<unsigned int>
	{
		static size_t Size_;
	public:
		UIntPrimitive(const char * Name, unsigned int Value);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API LongPrimitive : public Primitive<long long>
	{
		static size_t Size_;
	public:
		LongPrimitive(const char * Name, long long Value);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API ULongPrimitive : public Primitive<unsigned long long>
	{
		static size_t Size_;
	public:
		ULongPrimitive(const char * Name, unsigned long long Value);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API FloatPrimitive : public Primitive<float>
	{
		static size_t Size_;
	public:
		FloatPrimitive(const char * Name, float Value);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	class FLUFF_API DoublePrimitive : public Primitive<double>
	{
		static size_t Size_;
	public:
		DoublePrimitive(const char * Name, double Value);
		const size_t GetTypeSize() const override;
		virtual const char GetTypeHandle() const;
	};

	void FLUFF_API Write(IArchive * Archive, IPrimitive * Prim);
	void FLUFF_API Read(IArchive * Archive, IPrimitive ** Prim, size_t * Pointer);

} }
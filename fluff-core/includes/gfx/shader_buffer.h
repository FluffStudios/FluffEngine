#pragma once

#include <common.h>

namespace fluff { namespace gfx {

	struct FLUFF_API ShaderBufferDesc
	{
		unsigned int ByteWidth;
		unsigned int ReadWrite;
		unsigned int Usage;
		unsigned int AccessFlags;
		unsigned int MiscFlags;
		unsigned int StructureByteStride;
	};

	enum FLUFF_API READ_WRITE
	{
		MAP_WRITE = 0x0002,
		MAP_READ = 0x0001,
	};

	enum FLUFF_API USAGE
	{
		MAP_COHERENT = 0x00000080,
		MAP_PERSISTENT = 0x00000040
	};

	class FLUFF_API ShaderBuffer
	{
		uint32_t Id_;
		ShaderBufferDesc Descriptor_;
		void *Data_ = nullptr;
	public:
		/*
			Creates new shader buffer

			Descriptor - Describes data stored and access
		*/
		explicit ShaderBuffer(ShaderBufferDesc Descriptor);

		~ShaderBuffer() { Release(); }

		/*
			Sets the data to be stored

			Data - pointer to data stored
		*/
		void Set(void *Data) const;

		/*
			Writes data to buffer

			Data - pointer to data stored
			Offset - where to begin storing at
			Size - length of buffer
		*/
		void SubData(void *Data, int Offset, int Size);

		/*
			Maps a region of the buffer for usage

			Offset - where to begin read
			Size - length of read
			Returns buffer to store data in
		*/
		void* Map(int Offset, int Size) const;

		/*
			Unmaps all buffers
		*/
		static void Unmap();

		/*
			Gets handle of the shader buffer

			Returns shader buffer handle
		*/
		inline uint32_t GetHandle() const { return Id_; }

		/*
			Releases resources held by the buffer
		*/
		void Release();

		/*
			Enables buffer
		*/
		void Enable() const;

		/*
			Binds a range of data for reading in shader

			Index - Location to bind to
			Begin - Beginning offset
			Size - Size of data to bind
		*/
		void BindRange(int Index, int Begin, int Size) const;

		/*
			Disables all shader buffers
		*/
		static void Disable();
	};

} }

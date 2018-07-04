#pragma once

#include <common.h>

namespace fluff { namespace gfx {

	struct ShaderBufferDesc
	{
		unsigned int ByteWidth;
		unsigned int ReadWrite;
		unsigned int Usage;
		unsigned int AccessFlags;
		unsigned int MiscFlags;
		unsigned int StructureByteStride;
	};

	enum READ_WRITE
	{
		MAP_WRITE = 0x0002,
		MAP_READ = 0x0001,
	};

	enum USAGE
	{
		MAP_COHERENT = 0x00000080,
		MAP_PERSISTENT = 0x00000040
	};

	class ShaderBuffer
	{
		uint32_t Id_;
		ShaderBufferDesc Descriptor_;
		void *Data_ = nullptr;
	public:
		/*
			Creates new shader buffer

			Descriptor - Describes data stored and access
		*/
		explicit FLUFF_API ShaderBuffer(ShaderBufferDesc Descriptor);

		FLUFF_API ~ShaderBuffer() { Release(); }

		/*
			Sets the data to be stored

			Data - pointer to data stored
		*/
		void FLUFF_API Set(void *Data) const;

		/*
			Writes data to buffer

			Data - pointer to data stored
			Offset - where to begin storing at
			Size - length of buffer
		*/
		void FLUFF_API SubData(void *Data, int Offset, int Size);

		/*
			Maps a region of the buffer for usage

			Offset - where to begin read
			Size - length of read
			Returns buffer to store data in
		*/
		void FLUFF_API * Map(int Offset, int Size) const;

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
		void FLUFF_API Release();

		/*
			Enables buffer
		*/
		void FLUFF_API Enable() const;

		/*
			Binds a range of data for reading in shader

			Index - Location to bind to
			Begin - Beginning offset
			Size - Size of data to bind
		*/
		void FLUFF_API BindRange(int Index, int Begin, int Size) const;

		/*
			Disables all shader buffers
		*/
		static void Disable();
	};

} }

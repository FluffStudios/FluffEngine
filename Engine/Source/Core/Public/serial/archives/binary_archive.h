#pragma once

#include <common.h>

#include <serial/binary_buffer.h>

namespace fluff { namespace serial {

	class FLUFF_API BinaryArchive : public IArchive
	{
	public:
		BinaryArchive()
		{
			Buffer_ = new BinaryBuffer;
		}
		virtual Buffer * SendToBuffer() const { return Buffer_; }
		virtual void SendToFile(const char * FileName) {}
		virtual void ReadFromBuffer(Buffer * pBuffer) {}
		virtual void ReadFromFile(const char * FileName) {}
	};

} }
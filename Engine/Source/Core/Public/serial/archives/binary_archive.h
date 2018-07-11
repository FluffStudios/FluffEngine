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

	    Buffer * SendToBuffer() const override { return Buffer_; }
	    void SendToFile(const char * FileName) override {}
	    void ReadFromBuffer(Buffer * pBuffer) override {}
	    void ReadFromFile(const char * FileName) override {}
	};

} }
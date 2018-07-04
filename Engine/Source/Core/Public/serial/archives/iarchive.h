#pragma once

#include <common.h>

#include <serial/buffer.h>

namespace fluff { namespace serial {

	class FLUFF_API IArchive
	{
	protected:
		Buffer * Buffer_;
	public:
		virtual Buffer * SendToBuffer() const = 0;
		virtual void SendToFile(const char * FileName) = 0;
		virtual void ReadFromBuffer(Buffer * pBuffer) = 0;
		virtual void ReadFromFile(const char * FileName) = 0;
		virtual Buffer * GetBuffer() { return Buffer_; }
	};

} }
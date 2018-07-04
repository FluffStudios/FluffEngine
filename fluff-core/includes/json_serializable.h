#pragma once

#include <cereal\external\rapidjson\document.h>
#include <common.h>

namespace fluff {

	class JSONSerializable
	{
	public:
		virtual void FLUFF_API Serialize(rapidjson::Document & Doc, const char * Name) = 0;
		virtual JSONSerializable FLUFF_API * Deserialize(rapidjson::Document & Doc, const char * Name) = 0;
	};

}
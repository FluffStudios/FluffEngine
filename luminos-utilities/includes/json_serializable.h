#pragma once

#include <cereal\external\rapidjson\document.h>

namespace luminos {

	class JSONSerializable
	{
	public:
		virtual void Serialize(rapidjson::Document & Doc, const char * Name) = 0;
		virtual JSONSerializable* Deserialize(rapidjson::Document & Doc, const char * Name) = 0;
	};

}
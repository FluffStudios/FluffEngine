#pragma once

#include <common.h>

#include <serial/archives/iarchive.h>

#include <serial/types/arrays.h>
#include <serial/types/primitives.h>

#include <vector>

#include <memory>

namespace fluff { namespace serial {

	class ISerializable
	{
		char * Name_;
		size_t NameLength_;
		std::vector<std::shared_ptr<ISerializable>> Children_;
		std::vector<std::shared_ptr<IPrimitive>> Primitives_;
		std::vector<std::shared_ptr<IArray>> Arrays_;		
	public:
		FLUFF_API ISerializable(const char * Name);
		virtual FLUFF_API ~ISerializable();

		void AddChild(std::shared_ptr<ISerializable> & Child);
		void AddPrimitive(std::shared_ptr<IPrimitive> Prim);
		void AddArray(std::shared_ptr<IArray> & Arr);

		friend void FLUFF_API Write(IArchive * Archive, ISerializable * Obj);
		friend void FLUFF_API Read(IArchive * Archive, ISerializable ** Obj, size_t * Offset);
	};

	void FLUFF_API Write(IArchive * Archive, ISerializable * Obj);
	void FLUFF_API Read(IArchive * Archive, ISerializable ** Obj, size_t * Offset);

} }
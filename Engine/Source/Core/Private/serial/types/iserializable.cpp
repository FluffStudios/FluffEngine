#include <serial/types/iserializable.h>

#include <serial/types/types_defines.h>

namespace fluff { namespace serial {

	ISerializable::ISerializable(const char * Name)
	{
		NameLength_ = strlen(Name) + 1;
		Name_ = static_cast<char*>(calloc(NameLength_, sizeof(char)));
		strcpy_s(Name_, NameLength_, Name);
	}

	ISerializable::~ISerializable()
	{
		free(Name_);
	}

	void ISerializable::AddChild(std::shared_ptr<ISerializable> & Child)
	{
		Children_.push_back(Child);
	}

	void ISerializable::AddPrimitive(std::shared_ptr<IPrimitive> Prim)
	{
		Primitives_.push_back(Prim);
	}

	void ISerializable::AddArray(std::shared_ptr<IArray> & Arr)
	{
		Arrays_.push_back(Arr);
	}

	void Write(IArchive * Archive, ISerializable * Obj)
	{
		Archive->GetBuffer()->Add(OBJECT);
		Archive->GetBuffer()->Add(Obj->NameLength_);
		Archive->GetBuffer()->Add(Obj->Name_, Obj->NameLength_);
		Archive->GetBuffer()->Add(Obj->Primitives_.size());
		for (auto prim : Obj->Primitives_)
		{
			Write(Archive, prim.get());
		}
		Archive->GetBuffer()->Add(Obj->Arrays_.size());
		for (auto arr : Obj->Arrays_)
		{
			Write(Archive, arr.get());
		}
		Archive->GetBuffer()->Add(Obj->Children_.size());
		for (auto child : Obj->Children_)
		{
			Write(Archive, child.get());
		}
	}

	void Read(IArchive * Archive, ISerializable ** Obj, size_t * Offset)
	{
		char type;
		size_t nameLength;
		size_t primitiveCount;
		size_t arrayCount;
		size_t objectCount;
		char * name;

		size_t ptr = *Offset;

		ptr = Archive->GetBuffer()->Read(&type, ptr);
		FLUFF_ASSERT(type == OBJECT)

		ptr = Archive->GetBuffer()->Read(&nameLength, ptr);
		FLUFF_ASSERT(nameLength > 0)

		name = static_cast<char*>(calloc(nameLength, 1));
		ptr = Archive->GetBuffer()->Read(&name, ptr, nameLength);

		*Obj = new ISerializable(name);
		free(name);

		ptr = Archive->GetBuffer()->Read(&primitiveCount, ptr);
		for (size_t i = 0; i < primitiveCount; i++) 
		{
			IPrimitive * prim;
			Read(Archive, &prim, &ptr);
			(*Obj)->Primitives_.push_back(std::shared_ptr<IPrimitive>(prim));
		}

		ptr = Archive->GetBuffer()->Read(&arrayCount, ptr);
		for (size_t i = 0; i < arrayCount; i++)
		{
			IArray * arr;
			Read(Archive, &arr, &ptr);
			(*Obj)->Arrays_.push_back(std::shared_ptr<IArray>(arr));
		}

		ptr = Archive->GetBuffer()->Read(&objectCount, ptr);
		for (size_t i = 0; i < objectCount; i++)
		{
			ISerializable * obj;
			Read(Archive, &obj, &ptr);
			(*Obj)->Children_.push_back(std::shared_ptr<ISerializable>(obj));
		}
	}

}	}
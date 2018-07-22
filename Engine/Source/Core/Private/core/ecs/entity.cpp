#include <core/ecs/entity.h>
#include <serialization-legacy/serialization_utilities.h>

namespace fluff { namespace ecs {

	const ID Entity::INVALID;
	size_t IComponent::FamilyID_ = 0;

	Entity::Entity(std::shared_ptr<EntityManager> & Manager, ID Id)
		: Manager_(Manager), Id_(Id) 
	{ }

	Entity::operator bool() const
	{
		return this->IsValid();
	}

	bool Entity::operator == (const Entity & Other) const
	{
		return (Manager_ == Other.Manager_) && (Id_ == Other.Id_);
	}

	bool Entity::operator != (const Entity & Other) const
	{
		return (Manager_ != Other.Manager_) || (Id_ != Other.Id_);
	}

	bool Entity::operator < (const Entity & Other) const
	{
		return Id_ < Other.Id_;
	}

	bool Entity::IsValid() const
	{
		return Id_ != INVALID && Manager_ != nullptr;
	}

	void Entity::Invalidate()
	{
		Id_ = INVALID;
		Manager_ = nullptr;
	}

	ID Entity::GetID() const
	{
		return Id_;
	}

	void Entity::Destroy()
	{
		FLUFF_ASSERT(IsValid())
		Manager_->Destroy(Id_);
		Invalidate();
	}

	std::bitset<MAX_COMPONENT_COUNT> Entity::ComponentMask() const
	{
		return Manager_->ComponentMask(Id_);
	}

	void IComponentHelper::CopyComponentTo(Entity Src, Entity Dst)
	{
		Manager->CpyComponent(Dst.GetID(), ID, Size, Src.GetID().Index());
	}

	void IComponentHelper::RemoveComponent(Entity Ent)
	{
		Manager->RemoveComponentByID();
	}

	EntityManager::EntityManager(std::shared_ptr<EventManager> & EM) 
		: EventManager_(EM), IndexCounter_(0)
	{
		ThisPtr_ = std::shared_ptr<EntityManager>(this);
	}

	Entity EntityManager::Get(ID Id)
	{
		FLUFF_ASSERT(Id != Entity::INVALID)
		return Entity(ThisPtr_, Id);
	}

	ID EntityManager::CreateID(uint32_t Index) const
	{
		return ID(Index, EntityVersions_[Index]);
	}

	size_t EntityManager::GetSize() const
	{
		return EntityComponentMasks_.size() - FreeSlots_.size();
	}
	
	size_t EntityManager::GetCapacity() const
	{
		return EntityComponentMasks_.size();
	}

	bool EntityManager::IsValid(ID Id) const
	{
		return Id.Index() < EntityVersions_.size() && EntityVersions_[Id.Index()] == Id.Version();
	}

	Entity EntityManager::Create()
	{
		uint32_t index, version;
		if (FreeSlots_.empty())
		{
			index = IndexCounter_++;
			AccomodateEntity(index);
			version = EntityVersions_[index] = 1;
		}
		else
		{
			index = FreeSlots_.back();
			FreeSlots_.pop_back();
			version = EntityVersions_[index];
		}
		Entity entity(ThisPtr_, ID(index, version));
		EventManager_->EmitEvent<EntityCreatedEvent>(entity);
		return entity;
	}

	Entity EntityManager::CreateCopy(Entity Ent)
	{
		FLUFF_ASSERT(Ent.IsValid())
		auto clone = Create();
		auto mask = Ent.ComponentMask();
		for (size_t i = 0; i < ComponentHelpers_.size(); i++)
		{
			std::shared_ptr<IComponentHelper> helper = ComponentHelpers_[i];
			if (helper && mask.test(i))
			{
				helper->CopyComponentTo(Ent, clone);
			}
		}
		return clone;
	}

	void EntityManager::Destroy(ID Id)
	{
		FLUFF_ASSERT(this->IsValid(Id))

		const auto index = Id.Index();
		auto mask = EntityComponentMasks_[index];
		for (size_t i = 0; i < ComponentHelpers_.size(); i++)
		{
			std::shared_ptr<IComponentHelper> helper = ComponentHelpers_[i];
			if (helper && mask.test(i))
				helper->RemoveComponent(Entity(ThisPtr_, Id));
		}
		EventManager_->EmitEvent<EntityDestroyedEvent>(Entity(ThisPtr_, Id));
		EntityComponentMasks_[index].reset();
		EntityVersions_[index]++;
		FreeSlots_.push_back(index);
	}

	void EntityManager::AccomodateEntity(uint32_t Index)
	{
		if (EntityComponentMasks_.size() <= Index)
		{
			EntityComponentMasks_.resize(Index + 1);
			EntityVersions_.resize(Index + 1);
			for (auto pool : ComponentPools_)
			{
				if (pool) pool->Expand(Index + 1);
			}
		}
	}

	EntityCreatedEvent::~EntityCreatedEvent()
	{
	}

	EntityDestroyedEvent::~EntityDestroyedEvent()
	{
	}

} }
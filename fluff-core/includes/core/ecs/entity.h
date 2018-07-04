#pragma once

#include <common.h>
#include <non_copyable.h>

#include <core/ecs/event.h>
#include <core/ecs/event_manager.h>
#include <core/ecs/pool.h>
#include <core/ecs/system.h>
#include <core/lookup_table.h>

#include <bitset>
#include <algorithm>

#ifndef MAX_COMPONENT_COUNT
#define MAX_COMPONENT_COUNT 512
#endif

namespace fluff { namespace ecs {

	class EntityManager;

	template<typename Comp, typename EM = EntityManager>
	class ComponentHandle;

	class FLUFF_API ID
	{
		friend class EntityManager;
		uint64_t ID_;

		friend class cereal::access;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(ID_);
		}
	public:
		/*
			Creates default ID
		*/
		ID() : ID_(0) { }

		/*
			Creates an ID from a 64 bit integer

			Id - ID of entity
		*/
		explicit ID(const uint64_t Id) : ID_(Id) 
		{
			
		}

		/*
			Creates an ID from an index and a version

			Index - Offset for entity in pools
			Version - ECS Version
		*/
		ID(const uint32_t Index, const uint32_t Version) : ID_(uint64_t(Index) | uint64_t(Version) << 32UL) { }


		/*
			Gets the ID as a 64 bit integer

			Returns ID
		*/
		uint64_t GetID() const { return ID_; }
		
		/*
			Checks equality between IDs

			Other - other ID
			Returns if equivalent
		*/
		bool operator == (const ID& Other) const { return ID_ == Other.ID_; }

		/*
			Checks inequality between IDs

			Other - other ID
			Returns if not equivalent
		*/
		bool operator != (const ID& Other) const { return ID_ != Other.ID_; }

		/*
			Checks if ID is less than

			Other - other ID
			Returns if less tahn
		*/
		bool operator <  (const ID& Other) const { return ID_ < Other.ID_; }

		/*
			Gets the index of the ID

			Returns index
		*/
		uint32_t Index() const { return static_cast<uint32_t>(ID_); } // AND upper bits

		/*
			Gets the version of the ID

			Returns ID
		*/
		uint32_t Version() const { return ID_ >> 32; } // Shift to get version
	private:
	};

	class FLUFF_API Entity
	{
		ID Id_ = INVALID;
		EntityManager * Manager_;
	public:
		static const ID INVALID;
		/*
			Default constructor
		*/
		Entity() = default;

		/*
			Creates new entity from entity manager

			Manager - Reference to entity manager which dictates entity component behavior
			Id - ID of entity
		*/
		Entity(EntityManager * Manager, ID Id);

		/*
			Creates new entity from another

			Other - other entity
		*/
		Entity(const Entity & Other) = default;

		/*
			Creates new entity from another

			Other - other entity to copy from
			Returns new entity copy
		*/
		Entity & operator = (const Entity & Other) = default;

		/*
			Gets the entity's manager

			Returns entity's manager
		*/
		EntityManager * GetManager() const { return Manager_; }

		/*
			Checks if the entity's ID is valid

			Returns if entity is valid
		*/
		explicit operator bool() const;

		/*
			Checks if two entities are equivalent by ID

			Other - other entity
			Returns is equal
		*/
		bool operator == (const Entity &Other) const;

		/*
			Checks if two entities are not equivalent by ID

			Other - other entity
			Returns is not equal
		*/
		bool operator != (const Entity &Other) const;

		/*
			Checks if this entity is less that the other by ID

			Other - other entity
			Returns is less that
		*/
		bool operator <  (const Entity &Other) const;

		/*
			Checks if the entity' ID is valid

			Return is ID valid
		*/
		bool IsValid() const;

		/*
			Invalidates the entity's ID

			POSTCONDITION: IsValid() returns false
		*/
		void Invalidate();

		/*
			Gets the entity's ID

			Returns ID
		*/
		ID GetID() const;

		/*
			Adds a component to the entity

			Comp - Component Type
			Arguments - Types of parameters
			Args - Arguments passed
			Returns a handle to the component
		*/
		template<typename Comp, typename ... Arguments>
		ComponentHandle<Comp> AddComponent(Arguments && ... Args)
		{
			FLUFF_ASSERT(IsValid())
				return Manager_->AddComponent<Comp>(Id_, std::forward<Arguments>(Args)...);
		}

		/*
			Copies a component

			Comp - Type of component
			Type - Component to Copy
		*/
		template<typename Comp>
		ComponentHandle<Comp> CopyComponent(const Comp & Type)
		{
			FLUFF_ASSERT(IsValid())
				return Manager_->AddComponent<Comp>(Id_, Type);
		}

		/*
			Replaces a component.  If the component is not already
			contained, it adds the component

			Comp - Type of component
			Arguments - Types of parameters
			Args - Arguments passed
			Returns a handle to the component
		*/
		template<typename Comp, typename ... Arguments>
		ComponentHandle<Comp> ReplaceComponent(Arguments && ... Args)
		{
			FLUFF_ASSERT(IsValid())
				auto handle = GetComponent<Comp>();
			if (handle)
				*(handle.Get()) = Comp(std::forward<Arguments>(Args)...);
			else
				handle = Manager_->AddComponent<Comp>(Id_, std::forward<Arguments>(Args)...);
			return handle;
		}

		/*
			Removes component from entity

			Comp - Type of component to remove
		*/
		template<typename Comp>
		void RemoveComponent()
		{
			FLUFF_ASSERT(IsValid())
				Manager_->RemoveComponent<Comp>(Id_);
		}

		/*
			Get's the component handle for the type

			C - Component Type
			Returns component handle
		*/
		template<typename C, typename = typename std::enable_if<!std::is_const<C>::value>::type>
		ComponentHandle<C> GetComponent()
		{
			FLUFF_ASSERT(IsValid())
				return Manager_->Component<C>(Id_);
		}

		/*
			Get's the component handle for the type

			C - Component Type
			Returns component handle
		*/
		template<typename C, typename = typename std::enable_if<std::is_const<C>::value>::type>
		const ComponentHandle<C, const EntityManager> GetComponent() const
		{
			FLUFF_ASSERT(IsValid())
				return const_cast<const EntityManager*>(Manager_)->Component<const C>(Id_);
		}

		/*
			Gets tuple of components for the types provided

			Components - Component Types
			Returns a tuple of the component handles
		*/
		template<typename ... Components>
		std::tuple<ComponentHandle<Components>...> GetComponents()
		{
			FLUFF_ASSERT(IsValid())
				return Manager_->GetComponent<Components...>(Id_);
		}

		/*
			Gets tuple of components for the types provided

			Components - Component Types
			Returns a tuple of the component handles
		*/
		template<typename ... Components>
		std::tuple<ComponentHandle<const Components, const EntityManager>...> GetComponents() const
		{
			FLUFF_ASSERT(IsValid())
				return const_cast<const EntityManager*>(Manager_)->GetComponent<const Components...>(Id_);
		}

		/*
			Checks if entity has a component of the type provided

			Comp - Type of component
			Returns if entity has the component
		*/
		template<typename Comp>
		bool HasComponent() const
		{
			FLUFF_ASSERT(IsValid())
				return Manager_->HasComponent<Comp>(Id_);
		}

		/*
			Unpacks components to pointers

			A - A is type of component
			Arguments - Types of components
			Arg - Component Handle to unpack to
			Args - Component Handles to unpack to
		*/
		template<typename A, typename ... Arguments>
		void Unpack(ComponentHandle<A> &Arg, ComponentHandle<Arguments> & ... Args)
		{
			FLUFF_ASSERT(IsValid())
				Manager_->Unpack<A, Arguments>(Id_, Arg, Args ...);
		}

		/*
			Destroys the entity
		*/
		void Destroy();

		/*
			Gets the component mask of the entity

			Returns component mask
		*/
		std::bitset<MAX_COMPONENT_COUNT> ComponentMask() const;
	private:
	};

	template<typename ComponentType, typename EM>
	class FLUFF_API ComponentHandle
	{
		friend class EntityManager;
		EM * Manager_;
		ID Id_;

	public:
		/*
			Creates new component handle

			Manager - Entity manager associated with component
			Id - ID of entity owning the component
		*/
		ComponentHandle(EM * Manager, ID Id)
			: Manager_(Manager), Id_(Id)
		{ }

		/*
			Checks if the owning entity's ID is valid

			Returns if valid ID
		*/
		bool IsValid() const;

		/*
			Checks if the owning entity's ID is valid

			Returns if valid ID
		*/
		explicit operator bool() const;

		/*
			Allows access to contained pointer

			Returns encapsulated data
		*/
		ComponentType * operator -> ()
		{
			FLUFF_ASSERT(IsValid());
			return Manager_->template GetComponentPtr<ComponentType>(Id_);
		}

		/*
			Allows access to contained pointer

			Returns encapsulated data
		*/
		const ComponentType * operator -> () const
		{
			FLUFF_ASSERT(IsValid());
			return Manager_->template GetComponentPtr<ComponentType>(Id_);
		}

		/*
			Creates component handle with nothing attached
		*/
		ComponentHandle() 
			: Manager_(nullptr)
		{ }

		/*
			Gets pointer to the component

			Returns pointer to encapsulated data
		*/
		ComponentType * Get()
		{
			FLUFF_ASSERT(IsValid())
			return Manager_->template GetComponentPtr<ComponentType>(Id_);
		}

		/*
			Gets pointer to the component

			Returns pointer to encapsulated data
		*/
		const ComponentType * Get() const
		{
			FLUFF_ASSERT(IsValid())
			return Manager_->template GetComponentPtr<ComponentType>(Id_);
		}

		/*
			Removes itself from its associated entity
		*/
		void Remove()
		{
			FLUFF_ASSERT(IsValid())
			Manager_->template Remove<ComponentType>(Id_);
		}

		/*
			Gets the associated entity

			Returns associated entity
		*/
		Entity GetEntity()
		{
			FLUFF_ASSERT(IsValid())
			return Manager_->Get(Id_);
		}

		/*
			Checks if two component handles are equivalent

			Other - other component handle
			Returns if equivalent
		*/
		bool operator == (const ComponentHandle<ComponentType> & Other) const
		{
			return Manager_ == Other.Manager_ && Id_ == Other.Id_;
		}

		/*
			Checks if two component handles are not equivalent

			Other - other component handle
			Returns if not equivalent
		*/
		bool operator != (const ComponentHandle<ComponentType> & Other) const
		{
			return Manager_ != Other.Manager_ || Id_ != Other.Id_;
		}
	private:
	};

	struct FLUFF_API IComponent
	{
		void operator delete (void * Ptr) { Fail(); }
		void operator delete[] (void * Ptr) { Fail(); }

	protected:
		static void Fail()
		{
#if defined(_HAS_EXCEPTIONS) || defined(_EXCEPTIONS)
			throw std::bad_alloc();
#else
			std::abort();
#endif
		}
		static size_t FamilyID_;		
	};

	template<typename Type>
	struct FLUFF_API Component : public IComponent
	{
		typedef ComponentHandle<Type> Handle;
		typedef ComponentHandle<const Type, const EntityManager> ConstHandle;
		inline static size_t GetID() { return FamilyID_;  }
		inline static size_t GetFamilyID()
		{
			int64_t family = ComponentLookupTable::GetID<Component<Type>>();
			if (family == -1) { 
				family = ComponentLookupTable::GetNextSlot();
				FamilyID_ = family;
				ComponentLookupTable::Add<Component<Type>>();
			}
			FLUFF_ASSERT(family < MAX_COMPONENT_COUNT)
			return family;
		}
	private:
		friend class EntityManager;
	};

	struct FLUFF_API EntityCreatedEvent : public Event<EntityCreatedEvent>
	{
		explicit EntityCreatedEvent(Entity Ent) : E(Ent) { }
		virtual ~EntityCreatedEvent();

		Entity E;
	};

	struct FLUFF_API EntityDestroyedEvent : public Event<EntityDestroyedEvent>
	{
		explicit EntityDestroyedEvent(Entity Ent) : E(Ent) { }
		virtual ~EntityDestroyedEvent();

		Entity E;
	};

	template<typename Comp>
	struct FLUFF_API ComponentAddedEvent : public Event<ComponentAddedEvent<Comp>>
	{
		ComponentAddedEvent(const Entity Ent, ComponentHandle<Comp> Handle) : E(Ent), Handle(Handle) { }

		Entity E;
		ComponentHandle<Comp> Handle;
	};

	template<typename Comp>
	struct FLUFF_API ComponentRemovedEvent : public Event<ComponentRemovedEvent<Comp>>
	{
		ComponentRemovedEvent(const Entity Ent, ComponentHandle<Comp> Handle) : E(Ent), Handle(Handle) { }

		Entity E;
		ComponentHandle<Comp> Handle;
	};

	class FLUFF_API IComponentHelper
	{
	public:
		/*
			Deconstructor
		*/
		virtual ~IComponentHelper() { };
		/*
			Remove component helper

			Ent - Entity to remove from
		*/
		virtual void RemoveComponent(Entity Ent) = 0;
		
		/*
			Copy component helper

			Src - Entity to copy from
			Dst - Entity to copy to
		*/
		virtual void CopyComponentTo(Entity Src, Entity Dst) = 0;
	};

	template<typename Comp>
	class FLUFF_API ComponentHelper : public IComponentHelper
	{
	public:
		/*
			Component helper
		*/
		ComponentHelper() { }

		/*
			Remove component helper method

			Ent - Entity to remove from
		*/
		void RemoveComponent(Entity Ent) override;

		/*
			Copy component helper method

			Src - Entity to copy from
			Dst - Entity to copy to
		*/
		void CopyComponentTo(Entity Src, Entity Dst) override;
	};

	template <typename ComponentType, typename EM>
	bool ComponentHandle<ComponentType, EM>::IsValid() const
	{
		return (Id_ != Entity::INVALID) && (Manager_ != nullptr);
	}

	template <typename ComponentType, typename EM>
	ComponentHandle<ComponentType, EM>::operator bool() const
	{
		return IsValid();
	}

	template <typename Comp>
	void ComponentHelper<Comp>::RemoveComponent(Entity Ent)
	{
		Ent.RemoveComponent<Comp>();
	}

	template <typename Comp>
	void ComponentHelper<Comp>::CopyComponentTo(Entity Src, Entity Dst)
	{
		Dst.CopyComponent<Comp>(*(Src.GetComponent<Comp>().Get()));
	}

	class FLUFF_API EntityManager : NonCopyable
	{
		std::vector<std::bitset<MAX_COMPONENT_COUNT>> EntityComponentMasks_;
		std::vector<uint32_t> EntityVersions_;
		std::vector<IPool*> ComponentPools_;
		std::vector<IComponentHelper*> ComponentHelpers_;
		std::vector<uint32_t> FreeSlots_;
		EventManager & EventManager_;
		uint32_t IndexCounter_;
	public:
		/*
			Creates new Entity Manager

			EM - Event Manager for Entity Manager
		*/
		explicit EntityManager(EventManager & EM);

		/*
			Deconstructor
		*/
		virtual ~EntityManager() { Reset(); };

		template<class Type, bool All = false>
		class FLUFF_API ViewIterator : public std::iterator<std::input_iterator_tag, ID>
		{
		protected:
			EntityManager * Manager_;
			std::bitset<MAX_COMPONENT_COUNT> Mask_;
			uint32_t Index_;
			size_t Capacity_;
			size_t Cursor_;
		public:
			/*
				Prefix iterator

				Returns next value
			*/
			Type & operator ++()
			{
				++Index_;
				next();
				return *static_cast<Type*>(this);
			}

			/*
				Checks if two values are equivalent

				Returns if equivalent
			*/
			bool operator == (const Type & RHS) const { return Index_ == RHS.Index_; }

			/*
				Checks if two values are not equivalent

				Returns if not equivalent
			*/
			bool operator != (const Type & RHS) const { return Index_ != RHS.Index_; }

			/*
				Gets entity for index

				Returns entity
			*/
			Entity operator * () { return Entity(Manager_, Manager_->CreateID(Index_)); }

			/*
				Gets entity for index

				Returns entity
			*/
			Entity operator * () const { return Entity(Manager_, Manager_->CreateID(Index_)); }
		protected:
			ViewIterator(EntityManager * Manager, const std::bitset<MAX_COMPONENT_COUNT> Mask, const uint32_t Index)
				: Manager_(Manager), Mask_(Mask), Index_(Index), Capacity_(Manager_->GetCapacity()), Cursor_(~0UL)
			{ 
				if (All) {
					std::sort(Manager_->FreeSlots_.begin(), Manager_->FreeSlots_.end());
					Cursor_ = 0;
				}
			}


			ViewIterator(EntityManager *Manager, const uint32_t Index)
				: Manager_(Manager), Index_(Index), Capacity_(Manager_->GetCapacity()), Cursor_(~0UL) 
				{
				if (All) {
					std::sort(Manager_->FreeSlots_.begin(), Manager_->FreeSlots_.end());
					Cursor_ = 0;
				}

			}
			void next()
			{
				while (Index_ < Capacity_ && !Predicate())
				{
					++Index_;
				}
				if (Index_ < Capacity_)
				{
					auto ent = Manager_->Get(Manager_->CreateID(Index_));
					static_cast<Type*>(this)->next(ent);
				}
			}

			inline bool Predicate()
			{
				return (All && IsValidEntity()) || (Manager_->EntityComponentMasks_[Index_] & Mask_) == Mask_;
			}


			inline bool IsValidEntity()
			{
				const auto & free_list = Manager_->FreeSlots_;
				if (Cursor_ < free_list.size() && free_list[Cursor_] == Index_)
				{
					++Cursor_;
					return false;
				}
				return true;
			}
		};
		
		template<bool All>
		class FLUFF_API BaseView
		{
			friend class EntityManager;
			EntityManager * Manager_;
			std::bitset<MAX_COMPONENT_COUNT> Mask_;
		public:
			class FLUFF_API Iterator : public ViewIterator<Iterator, All> {
				public:
					/*
						Creates a new iterator

						Manager - Entity Manager for iterator
						Mask - Component Mask
						Index - Index of iterator
					*/
					Iterator(EntityManager * Manager, const std::bitset<MAX_COMPONENT_COUNT> Mask, const uint32_t Index)
						: ViewIterator<Iterator, All>(Manager, Mask, Index)
					{
						ViewIterator<Iterator, All>::next();
					}

					/*
						Advances iteration

						Ent - current entity
					*/
					void next(const Entity & Ent) { }
			};

			/*
				Gets the iterator for the first object

				Iterator of first object
			*/
			inline Iterator begin() { return Iterator(Manager_, Mask_, 0); }

			/*
				Gets the iterator for the last object

				Iterator of last object
			*/
			inline Iterator end() { return Iterator(Manager_, Mask_, static_cast<uint32_t>(Manager_->GetCapacity())); }

			/*
				Gets the iterator for the first object

				Iterator of first object
			*/
			inline const Iterator begin() const { return Iterator(Manager_, Mask_, 0); }

			/*
				Gets the iterator for the last object

				Iterator of last object
			*/
			inline const Iterator end() const { return Iterator(Manager_, Mask_, static_cast<uint32_t>(Manager_->GetCapacity())); }
		private:
			explicit BaseView(EntityManager * Manager) 
				: Manager_(Manager) 
			{ Mask_.set(); }
			BaseView(EntityManager * Manager, const std::bitset<MAX_COMPONENT_COUNT> Mask)
				: Manager_(Manager), Mask_(Mask)
			{ }
		};

		template<bool All, typename ... Components>
		class FLUFF_API TypedView : public BaseView<All>
		{
			friend class EntityManager;
			explicit TypedView(EntityManager * Manager) : BaseView<All>(Manager) { }
			TypedView(EntityManager * Manager, const std::bitset<MAX_COMPONENT_COUNT> Mask) 
				: BaseView<All>(Manager, Mask) { }
		public:
			template<typename Type> struct Identity { typedef Type type; };
			
			/*
				Performs a for each loop on the entities

				f - Function to perform
			*/
			void ForEach(typename Identity<std::function<void(Entity entity, Components & ...)>>::type f)
			{
				for (auto it : *this) f(it, *(it.template GetComponent<Components>().get())...);
			}
		};

		template<typename ... Components> using View = TypedView<false, Components ...>;
		typedef BaseView<true> DebugView;

		template<typename ... Components>
		class FLUFF_API UnpackingView
		{
		public:
			struct Unpacker
			{
				/*
					Creates an unpacker

					Handles - Component Handles
				*/
				explicit Unpacker(ComponentHandle<Components> & ... Handles) : Handles_(std::tuple<ComponentHandle<Components> & ...>(Handles...)) { }

				/*
					Unpacks the entity

					Ent - Entity to unpack
				*/
				void Unpack(Entity & Ent) const
				{
					UnpackHelper<0, Components...>(Ent);
				}
			private:
				template <int N, typename C>
				void UnpackHelper(Entity & Ent) const
				{
					std::get<N>(Handles_) = Ent.GetComponent<C>();
				}
				
				template <int N, typename C0, typename C1, typename ... Cn>
				void UnpackHelper(Entity & Ent) const
				{
					std::get<N>(Handles_) = Ent.GetComponent<C0>();
					UnpackHelper<N + 1, C1, Cn...>(Ent);
				}

				std::tuple<ComponentHandle<Components> & ...> Handles_;
			};

			class Iterator : public ViewIterator<Iterator>
			{
				const Unpacker & Unpacker_;
			public:
				/*
					Creates iterator implementation of view iterator

					Manager - entity manager for iterated entities
					Mask - component mask
					Index - index to start from
					Unpacker - unpacked components
				*/
				Iterator(EntityManager * Manager, const std::bitset<MAX_COMPONENT_COUNT> Mask, const uint32_t Index, const Unpacker &Unpack)
					: ViewIterator<Iterator>(Manager, Mask, Index), Unpacker_(Unpack)
				{ ViewIterator<Iterator>::next(); }

				/*
					Unpacks next entity

					Ent - entity to unpack
				*/
				void NextEntity(Entity & Ent) const
				{
					Unpacker_.Unpack(Ent);
				}
			};

			/*
				Gets first entity

				Returns first entity
			*/
			Iterator begin() { return Iterator(Manager_, Mask_, 0, Unpacker_); }

			/*
				Gets last entity

				Returns last entity
			*/
			Iterator end() { return Iterator(Manager_, Mask_, static_cast<uint32_t>(Manager_->GetCapacity()), Unpacker_); }
		private:
			friend class EntityManager;

			UnpackingView(EntityManager * Manager, const std::bitset<MAX_COMPONENT_COUNT> Mask, ComponentHandle<Components> & ... Handles)
				: Manager_(Manager), Mask_(Mask), Unpacker_(Handles)
			{ }

			EntityManager * Manager_;
			std::bitset<MAX_COMPONENT_COUNT> Mask_;
			Unpacker Unpacker_;
		};

		/*
			Gets the entity specified by the ID and associated
			with the EntityManager

			Id - ID of entity
			Returns entity associated with ID
		*/
		Entity Get(ID Id);

		/*
			Creates a new ID given the index

			Index - Index of new entity
			Returns new ID
		*/
		ID CreateID(uint32_t Index) const;

		/*
			Gets size of entity manager

			Returns size
		*/
		size_t GetSize() const;

		/*
			Gets the capacity of the entity manager

			Returns capacity
		*/
		size_t GetCapacity() const;

		/*
			Checks if ID is valid

			Return if valid
		*/
		bool IsValid(ID Id) const;

		/*
			Creates new Entity

			Returns new entity
		*/
		Entity Create();

		/*
			Creates a copy of a pre-existing entity

			Ent - Entity to copy
			Returns copy of entity
		*/
		Entity CreateCopy(Entity Ent);

		/*
			Destroys the entity with the ID given
		*/
		void Destroy(ID Id);

		/*
			Adds a component to the entity

			Comp - component type
			Arguments - parameter types
			Id - ID of entity
			Args - Arguments for component construction
			Returns component handle encapsulating type
		*/
		template<typename Comp, typename ... Arguments>
		ComponentHandle<Comp> AddComponent(ID Id, Arguments && ... Args)
		{
			FLUFF_ASSERT(IsValid(Id))
			const size_t family_id = ComponentFamilyID<Comp>();
			FLUFF_ASSERT(!EntityComponentMasks_[Id.Index()].test(family_id));

			Pool<Comp> * pool = AccomodateComponent<Comp>();
			::new(pool->GetAt(Id.Index())) Comp(std::forward<Arguments>(Args)...);

			EntityComponentMasks_[Id.Index()].set(family_id);
			ComponentHandle<Comp> component(this, Id);
			EventManager_.EmitEvent<ComponentAddedEvent<Comp>>(Entity(this, Id), component);
			return component;
		}

		/*
			Removes the component of given type

			Comp - type of component to remove
			Id - ID of entity to remove component from
		*/
		template<typename Comp>
		void RemoveComponent(ID Id)
		{
			FLUFF_ASSERT(IsValid(Id));
			const size_t family = ComponentFamilyID<Comp>();
			const auto index = Id.Index();
			auto pool = ComponentPools_[family];
			ComponentHandle<Comp> component(this, Id);
			EventManager_.EmitEvent<ComponentRemovedEvent<Comp>>(Entity(this, Id), component);
			EntityComponentMasks_[Id.Index()].reset(family);
			pool->Destroy(index);
		}

		/*
			Checks if entity has component of given type

			Comp - Type of component
			Id - ID of entity to check if it has component of given type
			Returns if entity contains component
		*/
		template<typename Comp>
		bool HasComponent(ID Id) const
		{
			FLUFF_ASSERT(IsValid(Id))

			const size_t family = ComponentFamilyID<Comp>();
			if (family >= ComponentPools_.size()) return false;
			const auto pool = ComponentPools_[family];
			if (!pool || !EntityComponentMasks_[Id.Index()][family]) return false;
			return true;
		}

		/*
			Gets component of type from entity

			Comp - Component Type
			Id - ID of entity
			Returns component handle
		*/
		template <typename Comp, typename = typename std::enable_if<!std::is_const<Comp>::value>::type>
		ComponentHandle<Comp> Component(ID Id)
		{
			FLUFF_ASSERT(IsValid(Id))
			
			const size_t family = ComponentFamilyID<Comp>();
			if (family >= ComponentPools_.size()) return ComponentHandle<Comp>();
			const auto pool = ComponentPools_[family];
			if (!pool || !EntityComponentMasks_[Id.Index()][family]) return ComponentHandle<Comp>();
			return ComponentHandle<Comp>(this, Id);
		}

		/*
			Gets component of type from entity

			Comp - Component Type
			Id - ID of entity
			Returns component handle
		*/
		template <typename C, typename = typename std::enable_if<std::is_const<C>::value>::type>
		ComponentHandle<C, const EntityManager> Component(ID Id) const {
			FLUFF_ASSERT(IsValid(Id))

			const size_t family = ComponentFamilyID<C>();
			if (family >= ComponentPools_.size()) return ComponentHandle<C, const EntityManager>();
			const auto pool = ComponentPools_[family];
			if (!pool || !EntityComponentMasks_[Id.Index()][family]) return ComponentHandle<C, const EntityManager>();
			return ComponentHandle<C, const EntityManager>(this, Id);
		}

		/*
			Gets tuple of components from entity

			Components - type of components to retreive
			Id - ID of entity
			Returns tuple of component handles
		*/
		template<typename ... Components>
		std::tuple<ComponentHandle<Components>...> Components(ID Id)
		{
			return std::make_tuple(Component<Components>(Id)...);
		}

		/*
			Gets tuple of components from entity

			Components - type of components to retreive
			Id - ID of entity
			Returns tuple of component handles
		*/
		template<typename ... Components>
		std::tuple<ComponentHandle<const Components, const EntityManager>...> Components(ID Id) const
		{
			return std::make_tuple(Component<Components>(Id)...);
		}

		/*
			Creates and returns an iterator of entities containing components of the provided type

			Components - Types to check for
			Return iterator of entities with components of type provided
		*/
		template<typename ... Components>
		View<Components ...> GetEntitiesWithComponents()
		{
			auto mask = ComponentMask<Components...>();
			return View<Components...>(this, mask);
		}

		template<typename Type> struct Identity { typedef Type type;};

		/*
			Calls a function on each entity in iterator 

			Components - Required component types in entity
		*/
		template<typename ... Components>
		void Each(typename Identity<std::function<void(Entity Ent, Components &...)>>::type Func)
		{
			return EntitiesWithComponents<Components...>().ForEach(Func);
		}

		/*
			Gets entities with components

			Components - Types of component
		*/
		template<typename ... Components>
		View<Components...> EntitiesWithComponents()
		{
			auto mask = ComponentMask<Components...>();
			return View<Components...>(this, mask);
		}

		/*
			Creates a debug view

			Returns a debugging view of entities
		*/
		DebugView DebugEntities()
		{
			return DebugView(this);
		}

		/* 
			Unpacks the entity

			Comp - component to unpack
			Id - ID of entity to unpack
			Handle - Component Handle to unpack to
		*/
		template<typename Comp>
		void Unpack(ID Id, ComponentHandle<Comp> &Handle)
		{
			FLUFF_ASSERT(IsValid(Id))
			Handle = Component<Comp>(Id);
		}

		/* 
			Unpacks the entity

			A - component to unpack
			Arguments - components to unpack
			Id - ID of entity to unpack
			Handle - Component Handle to unpack to
			Args - Component Handles to unpack to
		*/
		template<typename A, typename ... Arguments>
		void Unpack(ID Id, ComponentHandle<A> &Handle, ComponentHandle<Arguments> & ... Args)
		{
			FLUFF_ASSERT(IsValid(Id))
			Handle = Component<A>(Id);
			Unpack<Arguments ...>(Id, Args ...);
		}

		/*
			Resets the Entity Manager
		*/
		void Reset()
		{
			for (auto entity : DebugEntities()) entity.Destroy();
			for (auto *pool : ComponentPools_) {
				if (pool) delete pool;
			}
			for (auto *helper : ComponentHelpers_) {
				if (helper) delete helper;
			}
			ComponentPools_.clear();
			ComponentHelpers_.clear();
			EntityComponentMasks_.clear();
			EntityVersions_.clear();
			FreeSlots_.clear();
			IndexCounter_ = 0;
		}

		/*
			Gets the component family's ID

			Comp - component type
			Return family ID
		*/
		template<typename Comp>
		inline static size_t ComponentFamilyID()
		{
			return ecs::Component<typename std::remove_const<Comp>::type>::GetFamilyID();
		}

		/*
			Creates a new pool for a component

			Comp - new component type
			Returns pointer to pool
		*/
		template<typename Comp>
		Pool<Comp>* AccomodateComponent()
		{
			const size_t family = ComponentFamilyID<Comp>();
			if (ComponentPools_.size() <= family)
			{
				ComponentPools_.resize(family + 1, nullptr);
			}
			if (!ComponentPools_[family])
			{
				Pool<Comp> * pool = new Pool<Comp>();
				pool->Expand(IndexCounter_);
				ComponentPools_[family] = pool;
			}
			if (ComponentHelpers_.size() <= family)
			{
				ComponentHelpers_.resize(family + 1, nullptr);
			}
			if (!ComponentHelpers_[family])
			{
				ComponentHelper<Comp> * c = new ComponentHelper<Comp>();
				ComponentHelpers_[family] = static_cast<IComponentHelper*>(c);
			}
			return static_cast<Pool<Comp>*>(ComponentPools_[family]);
		}

		/*
			Gets component mask of entity

			Id - ID of entity
			Returns component mask
		*/
		std::bitset<MAX_COMPONENT_COUNT> ComponentMask(ID Id)
		{
			FLUFF_ASSERT(Id != Entity::INVALID)
			return EntityComponentMasks_.at(Id.Index());
		}

		/*
			Gets component mask of entity

			Id - ID of entity
			Returns component mask
		*/
		template<typename Comp>
		std::bitset<MAX_COMPONENT_COUNT> ComponentMask() const
		{
			std::bitset<MAX_COMPONENT_COUNT> mask;
			mask.set(ComponentFamilyID<Comp>());
			return mask;
		}

		/*
			Gets component mask

			C1 - Component 1
			C2 - Component 2
			Components - Remainder of components
			Returns component mask
		*/
		template <typename C1, typename C2, typename ... Components>
		std::bitset<MAX_COMPONENT_COUNT> ComponentMask() {
			return ComponentMask<C1>() | ComponentMask<C2, Components ...>();
		}

		/*
			Gets component mask

			C - Component
			Comp - Handle of component
			Returns component mask
		*/
		template <typename C>
		std::bitset<MAX_COMPONENT_COUNT> ComponentMask(const ComponentHandle<C> &Comp) {
			return ComponentMask<C>();
		}

		/*
			Gets component mask

			C1 - Component 1
			Components - Remainder of components
			Comp1 - Value of component 1
			Args - Value of remaining components
			Returns component mask
		*/
		template <typename C1, typename ... Components>
		std::bitset<MAX_COMPONENT_COUNT> ComponentMask(const ComponentHandle<C1> &Comp1, const ComponentHandle<Components> &... Args) {
			return ComponentMask<C1, Components ...>();
		}

		/*
			Gets pointer to encapsulated data

			Comp - component type
			id - ID of entity
			Returns pointer to encapsulated data
		*/
		template <typename Comp>
		Comp * GetComponentPtr(ID id) {
			FLUFF_ASSERT(IsValid(id))
			IPool *pool = ComponentPools_[ComponentFamilyID<Comp>()];
			FLUFF_ASSERT(pool)
			return static_cast<Comp*>(pool->GetAt(id.Index()));
		}

		/*
			Gets pointer to encapsulated data

			Comp - component type
			id - ID of entity
			Returns pointer to encapsulated data
		*/
		template <typename Comp>
		const Comp * GetComponentPtr(ID id) const {
			FLUFF_ASSERT(IsValid(id))
			IPool *pool = ComponentPools_[ComponentFamilyID<Comp>()];
			FLUFF_ASSERT(pool)
			return static_cast<const Comp*>(pool->GetAt(id.Index()));
		}

	private:
		void AccomodateEntity(uint32_t Index);
	};

} }
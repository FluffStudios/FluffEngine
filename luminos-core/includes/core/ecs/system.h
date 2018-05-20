#pragma once

#include <core/ecs/isystem.h>
#include <unordered_map>

namespace luminos { namespace ecs {

	class SystemManager;

	template<typename SystemType>
	class LUMINOS_API System : public ISystem
	{
	public:
		/*
			Destructor
		*/
		virtual ~System() { }
	private:
		friend class SystemManager;

		static size_t GetFamilyID()
		{
			static size_t family_id = FamilyIDCounter++;
			return family_id;
		}
	};

	class LUMINOS_API SystemManager : NonCopyable
	{
		std::unordered_map<size_t, std::shared_ptr<ISystem>> Systems_;
		bool Init_ = false;
		EntityManager & EntityManager_;
		EventManager & EventManager_;
		double LastUpdateTime_;
		double FixedRefreshTime_ = 1.0 / 30.0;
		double RefreshTime_ = 1.0 / 144.0;
	public:
		/*
			Creates system meanager

			Entities - EntityManager encapsulated
			Events - EventManager encapsulated
		*/
		SystemManager(EntityManager & Entities, EventManager & Events);

		/*
			Creates system meanager

			Entities - EntityManager encapsulated
			Events - EventManager encapsulated
			RefreshTime - Minimum time between fixed update calls
		*/
		SystemManager(EntityManager & Entities, EventManager & Events, double RefreshTime);
		
		/*
			Adds a system to the manager

			Type - Type of system
			System - Pointer to system
		*/
		template<typename Type>
		void AddSystem(const std::shared_ptr<Type> System);

		/*
			Adds a system to the manager

			Type - Type of system
			Arguments - Types of parameters
			Args - Arguments for system construction
		*/
		template<typename Type, typename ... Arguments>
		std::shared_ptr<Type> Add(Arguments & ... Args)
		{
			std::shared_ptr<Type> system(new Type(std::forward<Arguments>(Args)...));
			AddSystem(system);
			return system;
		}

		/*
			Gets a system given the type

			Type - Type of system
			Returns system pointer
		*/
		template<typename Type>
		std::shared_ptr<Type> GetSystem()
		{
			auto it = Systems_.find(Type::GetFamilyID());
			LUMINOS_ASSERT(it != Systems_.end())
			return it == Systems_.end() ? std::shared_ptr<Type>() : std::shared_ptr<Type>(std::static_pointer_cast<Type>(it->second));
		}

		/*
			Updates all systems

			Delta - Time between system updates
		*/
		void UpdateAll(double DeltaTime);

		/*
			Configures all systems
		*/
		void Configure();

		/*
			Sets refresh rate of system

			Hertz - Refresh rate
		*/
		void SetRefreshRate(const uint32_t Hertz)
		{
			this->RefreshTime_ = 1.0 / Hertz;
		}
	};

	template <typename Type>
	void SystemManager::AddSystem(const std::shared_ptr<Type> System)
	{
		Systems_.insert(std::pair<size_t, std::shared_ptr<Type>>(Type::GetFamilyID(), System));
	}
} }
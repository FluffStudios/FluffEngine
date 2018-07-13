#pragma once

#include <core/ecs/isystem.h>
#include <unordered_map>
#include <core/threading/thread_pool.h>
#include <core/debug/debug_log_writer.h>

namespace fluff { namespace ecs {

	class SystemManager;

	template<typename SystemType>
	class System : public ISystem
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

	class SystemManager : NonCopyable
	{
		std::unordered_map<size_t, std::shared_ptr<ISystem>> Systems_;
		bool Init_ = false;
		EntityManager & EntityManager_;
		EventManager & EventManager_;
		double LastUpdateTime_;
		double FixedRefreshTime_ = 1.0 / 30.0;
		double RefreshTime_ = 1.0 / 144.0;
		std::shared_ptr<threading::ThreadPool> Pool_;
	public:
		/*
			Creates system meanager

			Entities - EntityManager encapsulated
			Events - EventManager encapsulated
		*/
		FLUFF_API SystemManager(EntityManager & Entities, EventManager & Events);

		/*
			Creates system meanager

			Entities - EntityManager encapsulated
			Events - EventManager encapsulated
			RefreshTime - Minimum time between fixed update calls
		*/
		FLUFF_API SystemManager(EntityManager & Entities, EventManager & Events, double RefreshTime);
		
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
			if (it == Systems_.end()) 
			{
				FLUFF_LOG(debug::DebugErrorType::INVALID_PARAMETER, debug::DebugSeverity::FATAL, "Invalid Type");
				FLUFF_ASSERT(it != Systems_.end())
			}
			return it == Systems_.end() ? std::shared_ptr<Type>() : std::shared_ptr<Type>(std::static_pointer_cast<Type>(it->second));
		}

		/*
			Updates all systems

			Delta - Time between system updates
		*/
		void FLUFF_API UpdateAll(double DeltaTime);

		/*
			Configures all systems
		*/
		void  FLUFF_API Configure();

		/*
			Sets refresh rate of system

			Hertz - Refresh rate
		*/
		inline void SetRefreshRate(const uint32_t Hertz)
		{
			this->RefreshTime_ = 1.0 / Hertz;
		}

		std::shared_ptr<threading::ThreadPool> FLUFF_API & GetThreadPool();
	};

	template <typename Type>
	void SystemManager::AddSystem(const std::shared_ptr<Type> System)
	{
		Systems_.insert(std::pair<size_t, std::shared_ptr<Type>>(Type::GetFamilyID(), System));
	}
} }
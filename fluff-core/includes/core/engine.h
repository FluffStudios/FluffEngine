#pragma once

#include <core/ecs/ecs_manager.h>
#include <core/engine_settings.h>
#include <core/window.h>
#include <common.h>

namespace fluff
{
	class IGameState
	{
	protected:
		std::shared_ptr<ECSManager> pManager_;
		IGameState();
	public:
		~IGameState();
		virtual void FLUFF_API Configure() = 0;
		virtual void FLUFF_API Run() = 0;
		virtual void FLUFF_API Shutdown() = 0;
		virtual bool FLUFF_API ShouldShutdown() = 0;
		inline std::shared_ptr<ECSManager> & GetManager() { return pManager_; }
	};

	class Game
	{
		IGameState * ActiveState_;
	public:
		void FLUFF_API SetGameState(IGameState * GameState);
		void FLUFF_API PlayState() const;
		void FLUFF_API Shutdown();
	};

	class Engine
	{
		static struct LuminosEngine
		{
			EngineSettings Settings;
			Window * ActiveWindow;
		} Instance;

	public:
		static void Configure(const EngineSettings Settings);
		static void Play(Game * pGame, IGameState * State);
		static void Destroy();

		static WindowSettings& GetWindowSettings();
		static GFXSettings& GetGraphicsSettings();
	};
}
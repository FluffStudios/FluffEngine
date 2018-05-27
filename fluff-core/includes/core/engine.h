#pragma once

#include <core/ecs/ecs_manager.h>
#include <core/engine_settings.h>
#include <core/window.h>
#include <common.h>

namespace luminos
{
	class LUMINOS_API IGameState
	{
	protected:
		std::shared_ptr<ECSManager> pManager_;
		IGameState();
	public:
		~IGameState();
		virtual void Configure() = 0;
		virtual void Run() = 0;
		virtual void Shutdown() = 0;
		std::shared_ptr<ECSManager>& GetManager() { return pManager_; }
	};

	class LUMINOS_API Game
	{
		IGameState * ActiveState_;
	public:
		void SetGameState(IGameState * GameState);
		void PlayState() const;
		void Shutdown();
	};

	class LUMINOS_API Engine
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
#include <core/engine.h>
#include <core/timer.h>

namespace fluff
{
	void Game::SetGameState(IGameState * GameState)
	{
		this->ActiveState_ = GameState;
		this->ActiveState_->Configure();
	}

	void Game::PlayState() const
	{
		this->ActiveState_->GetManager()->GetSystemManager()->UpdateAll(Timer::DeltaTime());
		this->ActiveState_->Run();
	}

	void Game::Shutdown()
	{
		this->ActiveState_->Shutdown();
	}

	Engine::LuminosEngine Engine::Instance;

	void Engine::Configure(const EngineSettings Settings)
	{
		Instance.Settings = Settings;
		Instance.ActiveWindow = Window::CreateWindow(Settings.S_Window.Name.c_str(), 
			Settings.S_Window.Width, 
			Settings.S_Window.Height, 
			Settings.S_Window.VSync,
			Settings.S_Window.Fullscreen);
	}

	void Engine::Play(Game * pGame, IGameState * State)
	{
		pGame->SetGameState(State);
		while (Instance.ActiveWindow->Update() && !State->ShouldShutdown())
		{
			pGame->PlayState();
		}
		pGame->Shutdown();
	}

	void Engine::Destroy()
	{
		Instance.ActiveWindow->CloseWindow();
	}

	WindowSettings & Engine::GetWindowSettings()
	{
		return Instance.Settings.S_Window;
	}

	GFXSettings & Engine::GetGraphicsSettings()
	{
		return Instance.Settings.S_Graphics;
	}

	IGameState::IGameState()
	{
		pManager_ = std::shared_ptr<ECSManager>(new ECSManager);
	}

	IGameState::~IGameState()
	{
	}

}
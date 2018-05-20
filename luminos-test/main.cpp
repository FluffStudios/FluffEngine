#define INCLUDE_LUMINOS_UTILS
#define BREAK_ON_GL_ERROR

#include <core/registry.h>

#include "includes/ds_state.h"

int main()
{
	luminos::EngineSettings settings;
	settings.ReadFile("settings.json");
	luminos::Engine::Configure(settings);


	auto game_state = new DSState;

	luminos::Game game;

	Engine::Play(&game, game_state);

	luminos::Engine::Destroy();

	return 0;
}
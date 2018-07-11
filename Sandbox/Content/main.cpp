#define INCLUDE_FLUFF_UTILS
#define BREAK_ON_GL_ERROR
#define LUMINOS_API_EXPORT

#include "includes/ds_state.h"

int main()
{
	fluff::EngineSettings settings;
	settings.ReadFile("settings.json");
	fluff::Engine::Configure(settings);


	auto game_state = new DSState;

	fluff::Game game;

	Engine::Play(&game, game_state);

	fluff::Engine::Destroy();

	return 0;
}
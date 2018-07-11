#define INCLUDE_FLUFF_UTILS
#define BREAK_ON_GL_ERROR

#include "ds_state.h"

int main()
{
    EngineSettings settings;
    settings.ReadFile("settings.json");
	Engine::Configure(settings);

    const auto game_state = new DSState();

	Game game{};

	Engine::Play(&game, game_state);

	Engine::Destroy();

	return 0;
}
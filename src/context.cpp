#include "spec.h"

#include "context.h"

Context & Context::get_instance() {
	static Context render_context;
	return render_context;
}

Context::Context() : window(nullptr), renderer(nullptr) {
	// init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		throw std::runtime_error("Failed to init SDL " + std::string(SDL_GetError()));
	}

	// create window
	std::string title = Spec::get_instance().get_str("title");
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Spec::get_instance().get_int("view/width"), Spec::get_instance().get_int("view/height"), SDL_WINDOW_SHOWN);

	if (window == NULL)
		throw std::runtime_error("Failed to make window: " + std::string(SDL_GetError()));

	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL)
		throw std::runtime_error("Failed to make renderer");
}

Context::~Context() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

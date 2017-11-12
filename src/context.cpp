#include "spec.h"

#include "context.h"

Context & Context::get_instance() {
	static Context render_context;
	return render_context;
}

Context::Context() : window(nullptr), renderer(nullptr) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw (std::string("Failed to init SDL ") + SDL_GetError());
	}

	std::string title = Spec::get_instance().get_str("title");
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Spec::get_instance().get_int("width"), Spec::get_instance().get_int("height"), SDL_WINDOW_SHOWN);

	if (window == NULL)
		throw (std::string("Failed to make window: ") + SDL_GetError());

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL)
		throw std::string("Failed to make renderer");
}

Context::~Context() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

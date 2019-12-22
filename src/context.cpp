#include <stdexcept>

#include <SDL2/SDL_image.h>

#include "spec.h"

#include "context.h"

Context & Context::get_instance() {
	static Context render_context;
	return render_context;
}

Context::Context() : icon_path("textures"), window(nullptr), renderer(nullptr) {
	// init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		throw std::runtime_error("Failed to init SDL " + std::string(SDL_GetError()));
	}

	// create window
	window = SDL_CreateWindow(Spec::get_instance().get_str("title").c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Spec::get_instance().get_int("view/width"), Spec::get_instance().get_int("view/height"), SDL_WINDOW_SHOWN);

	if (!window)
		throw std::runtime_error("Failed to make window: " + std::string(SDL_GetError()));

	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (!renderer)
		throw std::runtime_error("Failed to make renderer");

	// add alpha blending
	if(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0)
		throw std::runtime_error("Failed to enable alpha blending: " + std::string(SDL_GetError()));
}

Context::~Context() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Context::reload() {
	SDL_SetWindowSize(window, Spec::get_instance().get_int("view/width"), Spec::get_instance().get_int("view/height"));

	SDL_Surface * icon = IMG_Load((icon_path + "/" + Spec::get_instance().get_str("icon")).c_str());

	SDL_SetWindowTitle(window, Spec::get_instance().get_str("title").c_str());
	SDL_SetWindowIcon(window, icon);

	SDL_FreeSurface(icon);
}

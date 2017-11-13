#include <SDL_image.h>

#include "spec.h"

#include "text.h"

Text & Text::get_instance() {
	static Text instance;
	return instance;
}

Text::~Text() {
	TTF_CloseFont(font);
	TTF_Quit();
}

Text::Text() {
	if (TTF_Init() < 0)
		throw std::string("Failed to initialize TTF");

	font = TTF_OpenFont(Spec::get_instance().get_str("font/file").c_str(), Spec::get_instance().get_int("font/size"));
	size = Spec::get_instance().get_int("font/size");

	if (!font)
		throw std::string("Failed to find font");
}


void Text::write(const std::string & text, int x, int y, SDL_Color color) const {
	SDL_Surface * surface = TTF_RenderText_Solid(font, text.c_str(), color);

	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

	int width = surface->w;
	int height = surface->h;

	SDL_FreeSurface(surface);

	SDL_Rect dst = {x, y, width, height};

	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_DestroyTexture(texture);
}

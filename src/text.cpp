#include "spec.h"

#include "text.h"

Text & Text::get_instance() {
	static Text text;
	return text;
}

Text::~Text() {
	TTF_CloseFont(font);
	TTF_Quit();
}

Text::Text() : path("fonts"), font(nullptr), size(-1) {
	// init TTF
	if (TTF_Init() < 0)
		throw std::runtime_error("Failed to initialize TTF");

	// open font
	font = TTF_OpenFont((path + "/" + Spec::get_instance().get_str("font/file")).c_str(), Spec::get_instance().get_int("font/size"));
	size = Spec::get_instance().get_int("font/size");

	if (!font)
		throw std::runtime_error("Failed to load font");
}

void Text::write(SDL_Renderer * renderer, const std::string & text, int x, int y, SDL_Color color) const {
	// write font at size
	SDL_Surface * surface = TTF_RenderText_Solid(font, text.c_str(), color);

	// create texture of text
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

	int width = surface->w;
	int height = surface->h;

	SDL_FreeSurface(surface);

	SDL_Rect dst = {x, y, width, height};

	// render texture
	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_DestroyTexture(texture);
}

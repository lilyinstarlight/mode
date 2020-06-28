#include <stdexcept>
#include <sstream>

#include "spec.h"

#include "text.h"

Text & Text::get_instance() {
	static Text text;
	return text;
}

Text::~Text() {
	// clean up after ourselves
	for (std::pair<std::string, TTF_Font *> font : _fonts)
		TTF_CloseFont(font.second);

	TTF_Quit();
}

Text::Text() : _path("fonts"), _fonts{} {
	// init TTF
	if (TTF_Init() < 0)
		throw std::runtime_error("Failed to initialize TTF");
}

void Text::write(SDL_Renderer * renderer, const std::string & name, const std::string & text, int x, int y, float rotation, SDL_Color color) {
	SDL_Surface * surface = write(name, text, color);
	render(renderer, surface, x, y, rotation);
	destroy(surface);
}

SDL_Surface * Text::write(const std::string & name, const std::string & text, SDL_Color color) {
	std::unordered_map<std::string, TTF_Font *>::const_iterator pos = _fonts.find(name);
	if (pos == _fonts.end()) {
		std::string filename = _path + "/" + Spec::get_instance().get_str(name + "/file");

		TTF_Font * font = TTF_OpenFont(filename.c_str(), Spec::get_instance().get_int(name + "/size"));
		if (!font)
			throw std::runtime_error("Failed to load font file " + filename);
		_fonts[name] = font;
	}

	std::vector<SDL_Surface *> surfaces;
	int width = 0;
	int height = 0;

	for (std::string & line : split(text, '\n')) {
		surfaces.push_back(TTF_RenderUTF8_Blended(_fonts[name], line.c_str(), color));

		SDL_SetSurfaceAlphaMod(surfaces.back(), color.a);

		if (surfaces.back()->w > width)
			width = surfaces.back()->w;
		height += surfaces.back()->h;
	}

	if (height == 0)
		return nullptr;

	SDL_Surface * combined = SDL_CreateRGBSurfaceWithFormat(0, width, height, surfaces.front()->format->BitsPerPixel, surfaces.front()->format->format);

	SDL_Rect dst = {0, 0, width, height};

	for (SDL_Surface * surface : surfaces) {
		SDL_BlitSurface(surface, nullptr, combined, &dst);
		dst.y += surface->h;
		SDL_FreeSurface(surface);
	}

	return combined;
}

void Text::render(SDL_Renderer * renderer, SDL_Surface * text, int x, int y, float rotation) const {
	// create texture of text
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, text);

	SDL_Rect dst = {x, y, text->w, text->h};

	// render texture
	SDL_RenderCopyEx(renderer, texture, nullptr, &dst, rotation, nullptr, SDL_FLIP_NONE);
	SDL_DestroyTexture(texture);
}

void Text::destroy(SDL_Surface * text) const {
	SDL_FreeSurface(text);
}

std::vector<std::string> Text::split(const std::string & string, char delimiter) const {
	std::vector<std::string> tokens;

	std::stringstream ss(string);

	std::string token;
	while (std::getline(ss, token, delimiter))
		tokens.push_back(token);

	return tokens;
}

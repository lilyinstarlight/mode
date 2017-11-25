#include <sstream>

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
    render(renderer, write(text, color), x, y);
}

SDL_Surface * Text::write(const std::string & text, SDL_Color color) const {
    std::vector<SDL_Surface *> surfaces;
    int width = 0;
    int height = 0;

    for (std::string & line : split(text, '\n')) {
	surfaces.push_back(TTF_RenderUTF8_Solid(font, line.c_str(), color));

	if (surfaces.back()->w > width)
	    width = surfaces.back()->w;
	height += surfaces.back()->h;
    }

    if (height == 0)
	return nullptr;

    Uint32 key;

    SDL_Surface * combined = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0xff);
    if (SDL_GetColorKey(surfaces.front(), &key) == 0)
	SDL_SetColorKey(combined, SDL_TRUE, key);

    SDL_Rect dst = {0, 0, width, height};

    for (SDL_Surface * surface : surfaces) {
	SDL_BlitSurface(surface, nullptr, combined, &dst);
	dst.y += surface->h;
	SDL_FreeSurface(surface);
    }

    return combined;
}

void Text::render(SDL_Renderer * renderer, SDL_Surface * text, int x, int y) const {
    // create texture of text
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, text);

    SDL_Rect dst = {x, y, text->w, text->h};
    SDL_FreeSurface(text);

    // render texture
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}

std::vector<std::string> Text::split(const std::string & string, char delimiter) const {
    std::vector<std::string> tokens;

    std::stringstream ss(string);

    std::string token;
    while (std::getline(ss, token, delimiter))
	tokens.push_back(token);

    return tokens;
}

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

Text::Text() : path("fonts"), font(nullptr), size(-1), padding(4) {
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
    int next = y;

    for (std::string & line : split(text, '\n'))
	next = write_line(renderer, line, x, next, color);
}

int Text::write_line(SDL_Renderer * renderer, const std::string & text, int x, int y, SDL_Color color) const {
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

    // return next line position
    return y + height + padding;
}

std::vector<std::string> Text::split(const std::string & string, char delimiter) const {
    std::vector<std::string> tokens;

    std::stringstream ss(string);

    std::string token;
    while (std::getline(ss, token, delimiter))
	tokens.push_back(token);

    return tokens;
}

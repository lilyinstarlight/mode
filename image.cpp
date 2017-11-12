#include "context.h"
#include "viewport.h"

#include "image.h"

Image::Image(SDL_Surface * surface, SDL_Texture * texture) : renderer(Context::get_instance().get_renderer()), surface(surface), texture(texture), view{0, 0, surface->w, surface->h} {}

Image::Image(const Image & image) : renderer(image.renderer), surface(image.surface), texture(image.texture), view(image.view) {}


Image & Image::operator=(const Image & image) {
	renderer = image.renderer;
	texture = image.texture;
	view = image.view;

	return *this;
}

void Image::draw(int x, int y) const {
	draw(x, y, 1.0f);
}

void Image::draw(int x, int y, float scale) const {
	x -= Viewport::get_instance().get_x();
	y -= Viewport::get_instance().get_y();

	SDL_Rect dest = {x, y, scale*view.h, scale*view.w};

	SDL_RenderCopy(renderer, texture, &view, &dest);
}

void Image::draw(int sx, int sy, int dx, int dy) const {
	SDL_Rect src = {sx, sy, view.w, view.h};
	SDL_Rect dst = {dx, dy, surface->w, surface->h};

	SDL_RenderCopy(renderer, texture, &src, &dst);
}

#include "context.h"
#include "viewport.h"

#include "image.h"

Image::Image(SDL_Surface * surface, SDL_Texture * texture) : renderer(Context::get_instance().get_renderer()), surface(surface), texture(texture) {}

Image::Image(const Image & image) : renderer(image.renderer), surface(image.surface), texture(image.texture) {}

Image & Image::operator=(const Image & image) {
	renderer = image.renderer;
	surface = image.surface;
	texture = image.texture;

	return *this;
}

void Image::draw(const Viewport & viewport, int x, int y) const {
	draw(viewport, x, y, 1.0f);
}

void Image::draw(const Viewport & viewport, int x, int y, float scale) const {
	SDL_Rect src, dest;

	// draw image from origin
	src.x = 0;
	src.y = 0;
	src.w = surface->w;
	src.h = surface->h;

	// draw within viewport
	dest.x = x - viewport.get_x();
	dest.y = y - viewport.get_y();
	dest.w = scale*surface->w;
	dest.h = scale*surface->h;

	// copy image into renderer
	SDL_RenderCopy(renderer, texture, &src, &dest);
}

void Image::draw(const SDL_Rect & src, const SDL_Rect & dst) const {
	// copy image into renderer
	SDL_RenderCopy(renderer, texture, &src, &dst);
}

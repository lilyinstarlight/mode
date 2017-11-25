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
	draw(viewport, x, y, 0.0f, 1.0f);
}

void Image::draw(const Viewport & viewport, int x, int y, float rotation, float scale) const {
	SDL_Rect src, dst;

	// draw image from origin
	src.x = 0;
	src.y = 0;
	src.w = surface->w;
	src.h = surface->h;

	// draw within viewport
	dst.x = x - viewport.get_x();
	dst.y = y - viewport.get_y();
	dst.w = scale*surface->w;
	dst.h = scale*surface->h;

	// draw image
	draw(src, dst, rotation);
}

void Image::draw(const SDL_Rect & src, const SDL_Rect & dst, float rotation) const {
	// copy image into renderer
	SDL_RenderCopyEx(renderer, texture, &src, &dst, rotation, nullptr, SDL_FLIP_NONE);
}

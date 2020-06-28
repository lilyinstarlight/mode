#include "context.h"
#include "viewport.h"

#include "image.h"

Image::Image(SDL_Surface * surface, SDL_Texture * texture) : _renderer(Context::get_instance().get_renderer()), _surface(surface), _texture(texture) {}

Image::Image(const Image & image) : _renderer(image._renderer), _surface(image._surface), _texture(image._texture) {}

void Image::draw(const Viewport & viewport, int x, int y) const {
	draw(viewport, x, y, 0.0f, 1.0f);
}

void Image::draw(const Viewport & viewport, int x, int y, float rotation, float scale) const {
	SDL_Rect src, dst;

	// draw image from origin
	src.x = 0;
	src.y = 0;
	src.w = _surface->w;
	src.h = _surface->h;

	// draw within viewport
	dst.x = x - viewport.get_x();
	dst.y = y - viewport.get_y();
	dst.w = scale*_surface->w;
	dst.h = scale*_surface->h;

	// draw image
	draw(src, dst, rotation);
}

void Image::draw(const SDL_Rect & src, const SDL_Rect & dst, float rotation) const {
	// copy image into renderer
	SDL_RenderCopyEx(_renderer, _texture, &src, &dst, rotation, nullptr, SDL_FLIP_NONE);
}

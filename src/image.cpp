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

void Image::draw(const Viewport & viewport, int x, int y) const {
	draw(viewport, x, y, 1.0f);
}

void Image::draw(const Viewport & viewport, int x, int y, float scale) const {
	// draw within viewport
	SDL_Rect dest = {static_cast<int>(x - viewport.get_x()), static_cast<int>(y - viewport.get_y()), static_cast<int>(scale*view.h), static_cast<int>(scale*view.w)};

	// copy image into renderer
	SDL_RenderCopy(renderer, texture, &view, &dest);
}

void Image::draw(const SDL_Rect & src, const SDL_Rect & dst) const {
	// copy image into renderer
	SDL_RenderCopy(renderer, texture, &src, &dst);
}

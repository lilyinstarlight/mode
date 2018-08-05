#include <cmath>

#include "context.h"
#include "spec.h"

#include "platform.h"

Platform::Platform(const std::string & name) : Body(name, true), tile(NONE), surface(SDL_CreateRGBSurface(0, Spec::get_instance().get_int(name + "/width"), Spec::get_instance().get_int(name + "/height"), get_image()->get_surface()->format->BitsPerPixel, get_image()->get_surface()->format->Rmask, get_image()->get_surface()->format->Gmask, get_image()->get_surface()->format->Bmask, get_image()->get_surface()->format->Amask)), size{0, 0, surface->w, surface->h} {
	std::string tiling = Spec::get_instance().get_str(name + "/tile");
	if (tiling == "none")
		tile = NONE;
	else if (tiling == "x")
		tile = X;
	else if (tiling == "y")
		tile = Y;
	else if (tiling == "both")
		tile = BOTH;
	else
		throw std::runtime_error("Invalid tiling: " + Spec::get_instance().get_str(get_name() + "/tile"));
}

Platform::Platform(const Platform & p) : Body(p), tile(p.tile), surface(SDL_ConvertSurface(p.surface, p.surface->format, 0)), size{0, 0, surface->w, surface->h} {}

Platform::~Platform() {
	SDL_FreeSurface(surface);
}

void Platform::draw(const Viewport & viewport) const {
	SDL_Texture * texture = SDL_CreateTextureFromSurface(Context::get_instance().get_renderer(), surface);

	SDL_Rect src, dst;

	// draw image from origin
	src.x = 0;
	src.y = 0;
	src.w = surface->w;
	src.h = surface->h;

	// draw within viewport
	dst.x = get_x() - viewport.get_x();
	dst.y = get_y() - viewport.get_y();
	dst.w = get_scale()*surface->w;
	dst.h = get_scale()*surface->h;

	SDL_RenderCopyEx(Context::get_instance().get_renderer(), texture, &src, &dst, get_rotation(), nullptr, SDL_FLIP_NONE);
}

void Platform::update(unsigned int ticks, World & world) {
	SDL_FreeSurface(surface);
	surface = SDL_CreateRGBSurface(0, get_width(), get_height(), get_image()->get_surface()->format->BitsPerPixel, get_image()->get_surface()->format->Rmask, get_image()->get_surface()->format->Gmask, get_image()->get_surface()->format->Bmask, get_image()->get_surface()->format->Amask);

	SDL_Rect src = {0, 0, get_image()->get_width(), get_height()};
	SDL_Rect dst = {0, 0, size.w, size.h};

	switch (tile) {
		case NONE:
			// draw adjusted background
			SDL_BlitSurface(get_image()->get_surface(), &src, surface, &dst);
			break;

		case X:
			// iterate over tiles in map
			for (int x = 0; x < size.w; x += get_image()->get_width()) {
				// draw adjusted tile
				dst.x = x;
				SDL_BlitSurface(get_image()->get_surface(), &src, surface, &dst);
			}

			break;

		case Y:
			// iterate over tiles in map
			for (int y = 0; y < size.h; y += get_image()->get_height()) {
				// draw adjusted tile
				dst.y = y;
				SDL_BlitSurface(get_image()->get_surface(), &src, surface, &dst);
			}

			break;

		case BOTH:
			// iterate over tiles in map
			for (int x = 0; x < size.w; x += get_image()->get_width()) {
				for (int y = 0; y < size.h; y += get_image()->get_height()) {
					// draw adjusted tile
					dst.x = x;
					dst.y = y;
					SDL_BlitSurface(get_image()->get_surface(), &src, surface, &dst);
				}
			}

			break;
	}

	Body::update(ticks, world);
}

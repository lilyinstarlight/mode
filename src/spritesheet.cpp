#include "spritesheet.h"

SDL_Surface* SpriteSheet::crop(const SDL_Surface * surface, const SDL_Rect & view) {
	// create new surface
	SDL_Surface * sub = SDL_CreateRGBSurfaceWithFormat(0, view.w, view.h, surface->format->BitsPerPixel, surface->format->format);

	// copy view of old surface into new surface
	SDL_BlitSurface(const_cast<SDL_Surface *>(surface), &view, sub, nullptr);

	return sub;
}

SDL_Surface * SpriteSheet::get(unsigned int i, unsigned int j) {
	if (i >= columns || j >= rows)
		return nullptr;

	// get view coordinates
	view.x = i*view.w;
	view.y = j*view.h;

	// crop to coordinates
	return crop(src, view);
}

SDL_Surface * SpriteSheet::get(unsigned int s) {
	// get index with column/row
	return get(s % columns, s/columns);
}

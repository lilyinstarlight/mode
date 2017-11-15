#include "spritesheet.h"

SDL_Surface* SpriteSheet::crop(SDL_Surface * surf, const SDL_Rect * view) {
	const SDL_PixelFormat * fmt = surf->format;

	// create new surface
	SDL_Surface * sub = SDL_CreateRGBSurface(0, view->w, view->h, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);

	// copy view of old surface into new surface
	SDL_BlitSurface(surf, view, sub, nullptr);

	return sub;
}

SDL_Surface * SpriteSheet::get(unsigned int i, unsigned int j) {
	if (i >= columns || j >= rows)
		return nullptr;

	// get view coordinates
	view.x = i*view.w;
	view.y = j*view.h;

	// crop to coordinates
	return crop(src, &view);
}

SDL_Surface * SpriteSheet::get(unsigned int s) {
	// get index with row/column
	return get(s % columns, s/columns);
}

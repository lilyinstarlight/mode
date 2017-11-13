#include "sheet.h"

SDL_Surface* SpriteSheet::crop(SDL_Surface * surf, const SDL_Rect * view) {
	const SDL_PixelFormat * fmt = surf->format;

	SDL_Surface * sub = SDL_CreateRGBSurface(0, view->w, view->h, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);

	SDL_BlitSurface(surf, view, sub, nullptr);

	return sub;
}

SDL_Surface * SpriteSheet::get(unsigned int i, unsigned int j){
	if (i >= columns || j >= rows)
		return nullptr;

	view.x = i*view.w;
	view.y = j*view.h;

	return crop(src, &view);
}

SDL_Surface * SpriteSheet::get(unsigned int s){
	return get(s % columns, s/columns);
}

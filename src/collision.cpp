#include <SDL2/SDL2_rotozoom.h>

#include "util.h"

#include "collision.h"

SDL_Rect CollisionStrategy::intersection(const Drawable & obj1, const Drawable & obj2) const {
	// get intersection points using max and min
	int x1 = Util::max(obj1.get_x(), obj2.get_x());
	int y1 = Util::max(obj1.get_y(), obj2.get_y());
	int x2 = Util::min(obj1.get_x() + obj1.get_width(), obj2.get_x() + obj2.get_width());
	int y2 = Util::min(obj1.get_y() + obj1.get_height(), obj2.get_y() + obj2.get_height());

	// calculate intersection size
	int w = x2 - x1;
	int h = y2 - y1;

	if (w > 0 && h > 0)
		// return non-zero intersection
		return {x1, y1, w, h};
	else
		// return empty intersection
		return {0, 0, 0, 0};
}

bool CollisionStrategy::visible(Uint32 pixel, const SDL_Surface * surface) const {
	// ignore pixels that are not 32-bit
	if (surface->format->BitsPerPixel != 32)
		return true;

	// check if pixel has full alpha value
	Uint32 pix = pixel & surface->format->Amask;
	pix >>= surface->format->Ashift;
	pix <<= surface->format->Aloss;

	return pix != 0;
}

bool NoneCollisionStrategy::check(const Drawable &, const Drawable &) const {
	// never collide
	return false;
}

bool RectangularCollisionStrategy::check(const Drawable & obj1, const Drawable & obj2) const {
	// intersect objects
	SDL_Rect area = intersection(obj1, obj2);

	// check for non-zero intersection
	return area.x != 0 || area.y != 0 || area.w != 0 || area.h != 0;
}

bool CircularCollisionStrategy::check(const Drawable & obj1, const Drawable & obj2) const {
	// get radii
	unsigned int r1 = Util::max(obj1.get_width(), obj1.get_height());
	unsigned int r2 = Util::max(obj2.get_width(), obj2.get_height());

	// get maximum collision distance
	float collision_distance = r1/2.0f + r2/2.0f;

	// get midpoints
	Vector2f mid1{obj1.get_x() + obj1.get_width()/2.0f, obj1.get_y() + obj1.get_height()/2.0f};
	Vector2f mid2{obj2.get_x() + obj2.get_width()/2.0f, obj2.get_y() + obj2.get_height()/2.0f};

	// check if midpoint distance is greater than maximum collision distance
	return Util::distance(mid1, mid2) <= collision_distance;
}

bool PixelCollisionStrategy::check(const Drawable & obj1, const Drawable & obj2) const {
	// intersect objects
	SDL_Rect area = intersection(obj1, obj2);

	// check for zero intersection
	if (area.x == 0 && area.y == 0 && area.w == 0 && area.h == 0)
		return false;

	// create new surfaces to perform calculations with
	SDL_Surface * s1 = rotozoomSurface(const_cast<SDL_Surface *>(obj1.get_surface()), -obj1.get_rotation(), obj1.get_scale(), SMOOTHING_OFF);
	SDL_Surface * s2 = rotozoomSurface(const_cast<SDL_Surface *>(obj2.get_surface()), -obj2.get_rotation(), obj2.get_scale(), SMOOTHING_OFF);

	// lock surfaces to prevent modification
	SDL_LockSurface(s1);
	SDL_LockSurface(s2);

	// grab pixels
	const Uint8 * pixels1 = static_cast<Uint8 *>(s1->pixels);
	const Uint8 * pixels2 = static_cast<Uint8 *>(s2->pixels);

	// for each pixel in the intersection
	for (int x = area.x; x < area.x + area.w; ++x) {
		for (int y = area.y; y < area.y + area.h; ++y) {
			// calculate pixel indices
			int idx1 = ((y - static_cast<int>(obj1.get_y()))*s1->pitch + (x - static_cast<int>(obj1.get_x()))*s1->format->BytesPerPixel);
			int idx2 = ((y - static_cast<int>(obj2.get_y()))*s2->pitch + (x - static_cast<int>(obj2.get_x()))*s2->format->BytesPerPixel);

			// get pixel
			const Uint32 * pix1 = static_cast<const Uint32 *>(static_cast<const void *>(pixels1 + idx1));
			const Uint32 * pix2 = static_cast<const Uint32 *>(static_cast<const void *>(pixels2 + idx2));

			// if both are visible report collision
			if (visible(*pix1, s1) && visible(*pix2, s2)) {
				SDL_UnlockSurface(s1);
				SDL_UnlockSurface(s2);

				SDL_FreeSurface(s1);
				SDL_FreeSurface(s2);

				return true;
			}
		}
	}

	// unlock and free surfaces
	SDL_UnlockSurface(s1);
	SDL_UnlockSurface(s2);

	SDL_FreeSurface(s1);
	SDL_FreeSurface(s2);

	// if no visible collision report no collision
	return false;
}

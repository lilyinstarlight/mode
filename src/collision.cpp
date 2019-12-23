#include <cmath>

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

	// return intersection (negative width and height for no intersection)
	return {x1, y1, w, h};
}

Uint32 CollisionStrategy::get_pixel(const SDL_Surface * surface, unsigned int x, unsigned int y) const {
    Uint8 * pix = (Uint8 *)surface->pixels + y*surface->pitch + x*surface->format->BytesPerPixel;

    switch(surface->format->BytesPerPixel) {
		case 1:
			return *pix;

		case 2:
			return *(Uint16 *)pix;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return pix[0] << 16 | pix[1] << 8 | pix[2];
			else
				return pix[0] | pix[1] << 8 | pix[2] << 16;

		case 4:
			return *(Uint32 *)pix;

		default:
			throw std::runtime_error("Unknown bytes per pixel: " + std::to_string(surface->format->BytesPerPixel));
    }

	// should never happen
	return 0;
}

bool CollisionStrategy::visible(Uint32 pixel, const SDL_PixelFormat * format) const {
	// check if pixel has non-transparent alpha value
	Uint8 red, green, blue, alpha;
	SDL_GetRGBA(pixel, format, &red, &green, &blue, &alpha);

	return alpha != 0;
}

Vector2f NoneCollisionStrategy::get(const Drawable &, const Drawable &) const {
	// never collide
	return Vector2f(0, 0);
}

bool NoneCollisionStrategy::check(const Drawable &, const Drawable &) const {
	// never collide
	return false;
}

Vector2f RectangularCollisionStrategy::get(const Drawable & obj1, const Drawable & obj2) const {
	// intersect objects
	SDL_Rect area = intersection(obj1, obj2);

	// check for collision
	if (area.w < 0 || area.h < 0)
		return Vector2f(0, 0);

	if (area.w < area.h) {
		if (obj1.get_x() < area.x)
			return Vector2f(1, 0);
		else
			return Vector2f(-1, 0);
	}
	else {
		if (obj1.get_y() < area.y)
			return Vector2f(0, 1);
		else
			return Vector2f(0, -1);
	}
}

bool RectangularCollisionStrategy::check(const Drawable & obj1, const Drawable & obj2) const {
	// intersect objects
	SDL_Rect area = intersection(obj1, obj2);

	// check for non-negative intersection
	return area.w >= 0 && area.h >= 0;
}

std::tuple<float, Vector2f, Vector2f> CircularCollisionStrategy::midpoints(const Drawable & obj1, const Drawable & obj2) const {
	// get radii
	unsigned int r1 = Util::max(obj1.get_width(), obj1.get_height());
	unsigned int r2 = Util::max(obj2.get_width(), obj2.get_height());

	// get maximum collision distance
	float collision_distance = r1/2.0f + r2/2.0f;

	// get midpoints
	Vector2f mid1{obj1.get_x() + obj1.get_width()/2.0f, obj1.get_y() + obj1.get_height()/2.0f};
	Vector2f mid2{obj2.get_x() + obj2.get_width()/2.0f, obj2.get_y() + obj2.get_height()/2.0f};

	return std::make_tuple(collision_distance, mid1, mid2);
}

Vector2f CircularCollisionStrategy::get(const Drawable & obj1, const Drawable & obj2) const {
	float collision_distance;
	Vector2f mid1, mid2;

	std::tie(collision_distance, mid1, mid2) = midpoints(obj1, obj2);

	// check for collision
	if (Util::distance(mid1, mid2) > collision_distance)
		return Vector2f(0, 0);

	// return midpoint difference
	return mid2 - mid1;
}

bool CircularCollisionStrategy::check(const Drawable & obj1, const Drawable & obj2) const {
	float collision_distance;
	Vector2f mid1, mid2;

	std::tie(collision_distance, mid1, mid2) = midpoints(obj1, obj2);

	// check if midpoint distance is less than maximum collision distance
	return Util::distance(mid1, mid2) <= collision_distance;
}

Vector2f PixelCollisionStrategy::get(const Drawable &, const Drawable &) const {
	return Vector2f(0, 0);
}

bool PixelCollisionStrategy::check(const Drawable & obj1, const Drawable & obj2) const {
	// intersect objects
	SDL_Rect area = intersection(obj1, obj2);

	// check for zero intersection
	if (area.w < 0 || area.h < 0)
		return false;

	// create new surfaces to perform calculations with
	SDL_Surface * s1 = rotozoomSurface(const_cast<SDL_Surface *>(obj1.get_surface()), -obj1.get_rotation(), obj1.get_scale(), SMOOTHING_OFF);
	SDL_Surface * s2 = rotozoomSurface(const_cast<SDL_Surface *>(obj2.get_surface()), -obj2.get_rotation(), obj2.get_scale(), SMOOTHING_OFF);

	// lock surfaces to prevent modification
	SDL_LockSurface(s1);
	SDL_LockSurface(s2);

	// for each pixel in the intersection
	for (int x = area.x; x < area.x + area.w; ++x) {
		for (int y = area.y; y < area.y + area.h; ++y) {
			// get pixels
			Uint32 pix1 = get_pixel(s1, x - static_cast<int>(obj1.get_x()), y - static_cast<int>(obj1.get_y()));
			Uint32 pix2 = get_pixel(s2, x - static_cast<int>(obj2.get_x()), y - static_cast<int>(obj2.get_y()));

			// if both are visible report collision
			if (visible(pix1, s1->format) && visible(pix2, s2->format)) {
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

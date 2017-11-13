#include "collision.h"

unsigned int CollisionStrategy::max(unsigned int left, unsigned int right) const {
	return right > left ? right : left;
}

unsigned int CollisionStrategy::min(unsigned int left, unsigned int right) const {
	return right < left ? right : left;
}

float CollisionStrategy::distance(const Vector2f & p1, const Vector2f & p2) const {
	return std::sqrt(std::pow(p2[0] - p1[0], 2) + std::pow(p2[1] - p1[1], 2));
}

SDL_Rect CollisionStrategy::intersection(const Drawable & obj1, const Drawable & obj2) const {
	int x1 = max(obj1.get_x(), obj2.get_x());
	int y1 = max(obj1.get_y(), obj2.get_y());
	int x2 = min(obj1.get_x() + obj1.get_width(), obj2.get_x() + obj2.get_width());
	int y2 = min(obj1.get_y() + obj1.get_height(), obj2.get_y() + obj2.get_height());

	int w = x2 - x1;
	int h = y2 - y1;

	if (w > 0 && h > 0)
		return {x1, y1, w, h};
	else
		return {0, 0, 0, 0};
}

bool CollisionStrategy::visible(Uint32 pixel, const SDL_Surface * surface) const {
	if (surface->format->BitsPerPixel != 32)
		return true;

	Uint32 pix = pixel & surface->format->Amask;
	pix >>= surface->format->Ashift;
	pix <<= surface->format->Aloss;

	return pix != 0;
}

bool NoneCollisionStrategy::check(const Drawable &, const Drawable &) const {
	return false;
}

bool RectangularCollisionStrategy::check(const Drawable & obj1, const Drawable & obj2) const {
	SDL_Rect area = intersection(obj1, obj2);

	return area.x != 0 || area.y != 0 || area.w != 0 || area.h != 0;
}

bool CircularCollisionStrategy::check(const Drawable & obj1, const Drawable & obj2) const {
	unsigned int r1 = max(obj1.get_width(), obj1.get_height());
	unsigned int r2 = max(obj2.get_width(), obj2.get_height());

	float collision_distance = r1/2.0f + r2/2.0f;

	Vector2f mid1{obj1.get_x() + obj1.get_width()/2.0f, obj1.get_y() + obj1.get_height()/2.0f};
	Vector2f mid2{obj2.get_x() + obj2.get_width()/2.0f, obj2.get_y() + obj2.get_height()/2.0f};

	return distance(mid1, mid2) <= collision_distance;
}

bool PixelCollisionStrategy::check(const Drawable & obj1, const Drawable & obj2) const {
	SDL_Rect area = intersection(obj1, obj2);

	SDL_Surface * s1 = const_cast<SDL_Surface *>(obj1.get_surface());
	SDL_Surface * s2 = const_cast<SDL_Surface *>(obj2.get_surface());

	SDL_LockSurface(s1);
	SDL_LockSurface(s2);

	const Uint32 * pixels1 = static_cast<Uint32 *>(s1->pixels);
	const Uint32 * pixels2 = static_cast<Uint32 *>(s2->pixels);

	for (int col = area.x; col < area.x + area.w; ++col) {
		for (int row = area.y; row < area.y + area.h; ++row) {
			int idx1 = s1->pitch*(row - obj1.get_y()) + (col - obj1.get_x());
			int idx2 = s2->pitch*(row - obj2.get_y()) + (col - obj2.get_x());

			if (idx1 < 0 || idx2 < 0)
				continue;

			Uint32 pix1 = pixels1[idx1];
			Uint32 pix2 = pixels2[idx2];

			if (visible(pix1, s1) && visible(pix2, s2)) {
				SDL_UnlockSurface(s1);
				SDL_UnlockSurface(s2);

				return true;
			}
		}
	}

	SDL_UnlockSurface(s1);
	SDL_UnlockSurface(s2);

	return false;
}

#ifndef COLLISION_H
#define COLLISION_H
#include <cmath>

#include "drawable.h"
#include "vector2f.h"

class CollisionStrategy {
	public:
		CollisionStrategy() {}
		virtual ~CollisionStrategy() {}

		virtual bool check(const Drawable & obj1, const Drawable & obj2) const = 0;

	protected:
		float distance(const Vector2f & p1, const Vector2f & p2) const;
		SDL_Rect intersection(const Drawable & obj1, const Drawable & obj2) const;
		bool visible(Uint32 pixel, const SDL_Surface * surface) const;
};

class NoneCollisionStrategy : public CollisionStrategy {
	public:
		NoneCollisionStrategy() {}

		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;
};

class RectangularCollisionStrategy : public CollisionStrategy {
	public:
		RectangularCollisionStrategy() {}

		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;
};

class CircularCollisionStrategy : public CollisionStrategy {
	public:
		CircularCollisionStrategy() {}

		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;
};

class PixelCollisionStrategy : public CollisionStrategy {
	public:
		PixelCollisionStrategy() {}

		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;
};
#endif

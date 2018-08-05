#ifndef COLLISION_H
#define COLLISION_H
#include <utility>

#include "drawable.h"
#include "vector2f.h"

class CollisionStrategy {
	public:
		CollisionStrategy() {}
		virtual ~CollisionStrategy() {}

		virtual Vector2f get(const Drawable & obj1, const Drawable & obj2) const = 0;
		virtual bool check(const Drawable & obj1, const Drawable & obj2) const = 0;

	protected:
		SDL_Rect intersection(const Drawable & obj1, const Drawable & obj2) const;
		bool visible(Uint32 pixel, const SDL_Surface * surface) const;
};

class NoneCollisionStrategy : public CollisionStrategy {
	public:
		NoneCollisionStrategy() {}

		virtual Vector2f get(const Drawable & obj1, const Drawable & obj2) const;
		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;
};

class RectangularCollisionStrategy : public CollisionStrategy {
	public:
		RectangularCollisionStrategy() {}

		virtual Vector2f get(const Drawable & obj1, const Drawable & obj2) const;
		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;
};

class CircularCollisionStrategy : public CollisionStrategy {
	public:
		CircularCollisionStrategy() {}

		virtual Vector2f get(const Drawable & obj1, const Drawable & obj2) const;
		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;

	private:
		std::tuple<float, Vector2f, Vector2f> midpoints(const Drawable & obj1, const Drawable & obj2) const;
};

class PixelCollisionStrategy : public CollisionStrategy {
	public:
		PixelCollisionStrategy() {}

		virtual Vector2f get(const Drawable & obj1, const Drawable & obj2) const;
		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;
};
#endif

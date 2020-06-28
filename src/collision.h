#ifndef COLLISION_H
#define COLLISION_H
#include <string>
#include <tuple>

#include <SDL.h>

#include "drawable.h"
#include "vector2f.h"

class CollisionStrategy {
	public:
		CollisionStrategy() {}
		virtual ~CollisionStrategy() {}

		CollisionStrategy(const CollisionStrategy &) {}
		const CollisionStrategy & operator=(const CollisionStrategy &) { return *this; }

		SDL_Rect intersection(const Drawable & obj1, const Drawable & obj2) const;

		virtual std::string get_name() const = 0;

		virtual Vector2f get(const Drawable & obj1, const Drawable & obj2) const = 0;
		virtual bool check(const Drawable & obj1, const Drawable & obj2) const = 0;

	protected:
		Uint32 get_pixel(const SDL_Surface * surface, unsigned int x, unsigned int y) const;
		bool visible(Uint32 pixel, const SDL_PixelFormat * format) const;
};

class NoneCollisionStrategy : public CollisionStrategy {
	public:
		NoneCollisionStrategy() {}
		virtual ~NoneCollisionStrategy() {}

		NoneCollisionStrategy(const NoneCollisionStrategy &) : CollisionStrategy() {}
		const NoneCollisionStrategy & operator=(const NoneCollisionStrategy &) { return *this; }

		virtual std::string get_name() const { return "none"; }

		virtual Vector2f get(const Drawable & obj1, const Drawable & obj2) const;
		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;
};

class RectangularCollisionStrategy : public CollisionStrategy {
	public:
		RectangularCollisionStrategy() {}
		virtual ~RectangularCollisionStrategy() {}

		RectangularCollisionStrategy(const RectangularCollisionStrategy &) : CollisionStrategy() {}
		const RectangularCollisionStrategy & operator=(const RectangularCollisionStrategy &) { return *this; }

		virtual std::string get_name() const { return "rectangular"; }

		virtual Vector2f get(const Drawable & obj1, const Drawable & obj2) const;
		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;
};

class CircularCollisionStrategy : public CollisionStrategy {
	public:
		CircularCollisionStrategy() {}
		virtual ~CircularCollisionStrategy() {}

		CircularCollisionStrategy(const CircularCollisionStrategy &) : CollisionStrategy() {}
		const CircularCollisionStrategy & operator=(const CircularCollisionStrategy &) { return *this; }

		virtual std::string get_name() const { return "circular"; }

		virtual Vector2f get(const Drawable & obj1, const Drawable & obj2) const;
		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;

	private:
		std::tuple<float, Vector2f, Vector2f> midpoints(const Drawable & obj1, const Drawable & obj2) const;
};

class PixelCollisionStrategy : public CollisionStrategy {
	public:
		PixelCollisionStrategy() {}
		virtual ~PixelCollisionStrategy() {}

		PixelCollisionStrategy(const PixelCollisionStrategy &) : CollisionStrategy() {}
		const PixelCollisionStrategy & operator=(const PixelCollisionStrategy &) { return *this; }

		virtual std::string get_name() const { return "pixel"; }

		virtual Vector2f get(const Drawable & obj1, const Drawable & obj2) const;
		virtual bool check(const Drawable & obj1, const Drawable & obj2) const;
};
#endif

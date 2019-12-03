#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <string>

#include "pool.h"
#include "sprite.h"
#include "vector2f.h"

class Projectile : public Sprite {
	public:
		Projectile(const std::string & name);
		Projectile(const std::string & name, Pool<Projectile> & p);
		Projectile(const Projectile & projectile);
		virtual ~Projectile() {}

		const Projectile & operator=(const Projectile &) = delete;

		virtual void dispatch(const SDL_Event & event);
		virtual void update(unsigned int ticks, World & world);

		Vector2f get_origin() const         { return origin;           }
		void set_origin()                   { origin = get_position(); }
		void set_origin(const Vector2f & o) { origin = o;              }

		void create();
		void destroy();

	private:
		Pool<Projectile> * pool;

		Vector2f origin;
};
#endif

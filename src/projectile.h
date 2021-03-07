#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <string>

#include "pool.h"
#include "sprite.h"
#include "vector2f.h"

class Projectile : public Sprite {
	public:
		Projectile(const std::string & name);
		Projectile(const std::string & name, Pool<Projectile> & pool);
		Projectile(const Projectile & projectile);
		virtual ~Projectile() {}

		const Projectile & operator=(const Projectile &) = delete;

		virtual void dispatch(const SDL_Event & event);
		virtual void update(unsigned int ticks, World & world);

		const Vector2f & get_origin() const      { return _origin;           }
		Vector2f & get_mutable_origin()          { return _origin;           }
		void set_origin(const Vector2f & origin) { _origin = origin;         }
		void reset_origin()                      { _origin = get_position(); }

		virtual void revive();

	private:
		Pool<Projectile> * _pool;

		Vector2f _origin;
};
#endif

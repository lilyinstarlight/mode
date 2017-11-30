#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <string>

#include "sprite.h"
#include "vector2f.h"

class Projectile : public Sprite {
	public:
		Projectile(const std::string & name);
		Projectile(const Projectile & projectile);
		virtual ~Projectile() {}

		const Projectile & operator=(const Projectile &) = delete;

		bool is_alive() const { return alive; }

		Vector2f get_origin() const { return origin; }

		void create();
		void destroy();

	private:
		bool alive;

		Vector2f origin;
};
#endif

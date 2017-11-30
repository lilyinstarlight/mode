#include "util.h"

#include "projectile.h"

Projectile::Projectile(const std::string & name) : Sprite(name), alive(false), origin{0, 0} {}

Projectile::Projectile(const Projectile & projectile) : Sprite(projectile), alive(projectile.alive), origin(projectile.origin) {}

void Projectile::create() {
	alive = true;

	origin = get_position();
}

void Projectile::destroy() {
	alive = false;
}

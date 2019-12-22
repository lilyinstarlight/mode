#include "engine.h"
#include "util.h"
#include "world.h"

#include "projectile.h"

Projectile::Projectile(const std::string & name) : Sprite(name, true), pool(nullptr), origin{0, 0} {}

Projectile::Projectile(const std::string & name, Pool<Projectile> & p) : Sprite(name, true), pool(&p), origin{0, 0} {}

Projectile::Projectile(const Projectile & projectile) : Sprite(projectile), pool(projectile.pool), origin(projectile.origin) {}

void Projectile::dispatch(const SDL_Event & event) {
	if (pool && !is_alive())
		pool->destroy(*this);

	Sprite::dispatch(event);

	if (pool && !is_alive())
		pool->destroy(*this);
}

void Projectile::update(unsigned int ticks, World & world) {
	if (pool && !is_alive())
		pool->destroy(*this);

	Sprite::update(ticks, world);

	if (pool && !is_alive()) {
		world.remove(*this);
		pool->destroy(*this);
	}
}

void Projectile::revive() {
	Sprite::revive();

	origin = get_position();
}

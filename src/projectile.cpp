#include "engine.h"
#include "util.h"
#include "world.h"

#include "projectile.h"

Projectile::Projectile(const std::string & name) : Sprite(name, true), _pool(nullptr), _origin{0, 0} {}

Projectile::Projectile(const std::string & name, Pool<Projectile> & pool) : Sprite(name, true), _pool(&pool), _origin{0, 0} {}

Projectile::Projectile(const Projectile & projectile) : Sprite(projectile), _pool(projectile._pool), _origin(projectile._origin) {}

void Projectile::dispatch(const SDL_Event & event) {
	if (_pool && !is_alive())
		_pool->destroy(*this);

	Sprite::dispatch(event);

	if (_pool && !is_alive())
		_pool->destroy(*this);
}

void Projectile::update(unsigned int ticks, World & world) {
	if (_pool && !is_alive())
		_pool->destroy(*this);

	Sprite::update(ticks, world);

	if (_pool && !is_alive()) {
		world.remove(*this);
		_pool->destroy(*this);
	}
}

void Projectile::revive() {
	Sprite::revive();

	_origin = get_position();
}

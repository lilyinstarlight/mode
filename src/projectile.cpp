#include "engine.h"
#include "util.h"
#include "world.h"

#include "projectile.h"

Projectile::Projectile(const std::string & name) : Sprite(name, true), pool(Engine::get_instance().get_world().get_player().get_pool()), origin{0, 0} {}

Projectile::Projectile(const std::string & name, Pool<Projectile> & p) : Sprite(name, true), pool(p), origin{0, 0} {}

Projectile::Projectile(const Projectile & projectile) : Sprite(projectile), pool(projectile.pool), origin(projectile.origin) {}

void Projectile::dispatch(const SDL_Event & event) {
	if (!is_alive())
		pool.destroy(*this);

	Sprite::dispatch(event);

	if (!is_alive())
		pool.destroy(*this);
}

void Projectile::update(unsigned int ticks, World & world) {
	if (!is_alive())
		pool.destroy(*this);

	Sprite::update(ticks, world);

	if (!is_alive()) {
		world.remove(*this);
		pool.destroy(*this);
	}
}

void Projectile::create() {
	revive();

	origin = get_position();

	signal("create");
}

void Projectile::destroy() {
	kill();

	signal("destroy");
}

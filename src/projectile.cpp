#include "engine.h"
#include "util.h"
#include "world.h"

#include "projectile.h"

Projectile::Projectile(const std::string & name) : Sprite(name), pool(Engine::get_instance().get_world().get_player().get_pool()), alive(false), origin{0, 0} {}

Projectile::Projectile(const std::string & name, Pool<Projectile> & p) : Sprite(name), pool(p), alive(false), origin{0, 0} {}

Projectile::Projectile(const Projectile & projectile) : Sprite(projectile), pool(projectile.pool), alive(projectile.alive), origin(projectile.origin) {}

void Projectile::dispatch(const SDL_Event & event) {
	if (!is_alive())
		pool.destroy(*this);

	Sprite::dispatch(event);

	if (!is_alive())
		pool.destroy(*this);
}

void Projectile::update(unsigned int ticks) {
	if (!is_alive())
		pool.destroy(*this);

	Sprite::update(ticks);

	if (!is_alive())
		pool.destroy(*this);
}

void Projectile::create() {
	alive = true;
	origin = get_position();

	Engine::get_instance().get_world().add(*this);

	signal("create");
}

void Projectile::destroy() {
	alive = false;

	signal("destroy");

	Engine::get_instance().get_world().remove(*this);
}

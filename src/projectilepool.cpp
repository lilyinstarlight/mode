#include "engine.h"

#include "projectilepool.h"

ProjectilePool & ProjectilePool::get_instance() {
	static ProjectilePool projectile_pool;
	return projectile_pool;
}

ProjectilePool::ProjectilePool() {
}

ProjectilePool::~ProjectilePool() {
}

const Sprite & ProjectilePool::create() {
	return Engine::get_instance().get_world().get_player();
}

void ProjectilePool::destroy(const Sprite &) {
}

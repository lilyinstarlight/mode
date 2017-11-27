#include "projectilepool.h"

static ProjectilePool & ProjectilePool::get_instance() {
	static ProjectilePool projectile_pool;
	return projectile_pool;
}

ProjectilePool::ProjectilePool() {
}

ProjectilePool::~ProjectilePool() {
}

const Sprite & ProjectilePool::create() {
}

void ProjectilePool::destroy(const Sprite &) {
}

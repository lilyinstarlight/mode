#include "spec.h"

#include "world.h"

//TODO: draw backgrounds

World::World() :
    width(Spec::get_instance().get_int("world/width")),
    height(Spec::get_instance().get_int("world/height")) {}

void World::update(unsigned int) {
}

void World::draw(const Viewport &) const {
}


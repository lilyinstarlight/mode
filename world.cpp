#include "spec.h"

#include "world.h"

World::World() :
    width(Spec::get_instance().get_int("world/width")),
    height(Spec::get_instance().get_int("world/height")) {}

void World::update() {
}

void World::draw() const {
}


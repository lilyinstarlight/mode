#include <string>

#include "background.h"
#include "sprite.h"
#include "spec.h"

#include "world.h"

World::World() : width(Spec::get_instance().get_int("world/width")), height(Spec::get_instance().get_int("world/height")), player(), drawables{&player} {
	// get top level elements and add applicable ones to drawables
	for (const std::string & str : Spec::get_instance().get_tops()) {
		if (Spec::get_instance().check(str + "/type")) {
			if (Spec::get_instance().get_str(str + "/type") == "background") {
				drawables.insert(new Background(str));
			}
			else if (Spec::get_instance().get_str(str + "/type") == "sprite") {
				drawables.insert(new Sprite(str));
			}
		}
	}
}

World::~World() {
	// free added drawables
	for (Drawable * drawable : drawables) {
		if (drawable != &player)
			delete drawable;
	}
}

void World::add(Drawable * drawable) {
	drawables.insert(drawable);
}

void World::remove(Drawable * drawable) {
	drawables.erase(drawable);
}

void World::update(unsigned int ticks) {
	// update drawables
	for (Drawable * drawable : drawables) {
		drawable->update(ticks);

		// push away from edges
		if (drawable->get_y() < 0)
		  drawable->set_velocity_y(std::abs(drawable->get_velocity_y()));
		else if (drawable->get_y() > height - drawable->get_height())
		  drawable->set_velocity_y(-std::abs(drawable->get_velocity_y()));

		if (drawable->get_x() < 0)
		  drawable->set_velocity_x(std::abs(drawable->get_velocity_y()));
		else if (drawable->get_x() > width - drawable->get_width())
		  drawable->set_velocity_x(-std::abs(drawable->get_velocity_y()));
	}
}

void World::draw(const Viewport & viewport) const {
	// draw ordered drawables
	for (Drawable * drawable : drawables)
		drawable->draw(viewport);
}

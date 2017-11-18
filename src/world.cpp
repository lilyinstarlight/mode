#include <cmath>
#include <string>

#include "background.h"
#include "sound.h"
#include "sprite.h"
#include "spec.h"

#include "world.h"

World::World() : width(Spec::get_instance().get_int("world/width")), height(Spec::get_instance().get_int("world/height")), player(nullptr), drawables{} {}

void World::init() {
	player = new Player();
	drawables.insert(player);

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

	Sound::get_instance().play("theme");
}

World::~World() {
	// free added drawables
	for (Drawable * drawable : drawables) {
		delete drawable;
	}
}

void World::add(Drawable * drawable) {
	drawables.insert(drawable);
}

void World::remove(Drawable * drawable) {
	drawables.erase(drawable);
}

void World::draw(const Viewport & viewport) const {
	// draw ordered drawables
	for (Drawable * drawable : drawables)
		drawable->draw(viewport);
}

void World::update(unsigned int ticks) {
	// update drawables
	for (Drawable * drawable : drawables) {
		drawable->update(ticks);

		// set position based on velocity delta
		Vector2f pos = drawable->get_position() + drawable->get_velocity()*ticks/1000;
		drawable->set_position(pos);

		// push away from edges
		if (drawable->get_x() < 0 && drawable->get_velocity_x() < 0) {
			drawable->set_x(0);
			drawable->set_velocity_x(0);
		}
		else if (drawable->get_x() > width - drawable->get_width() && drawable->get_velocity_x() > 0) {
			drawable->set_x(width - drawable->get_width());
			drawable->set_velocity_x(0);
		}

		if (drawable->get_y() < 0 && drawable->get_velocity_y() < 0) {
			drawable->set_y(0);
			drawable->set_velocity_y(0);
		}
		else if (drawable->get_y() > height - drawable->get_height() && drawable->get_velocity_y() > 0) {
			drawable->set_y(height - drawable->get_height());
			drawable->set_velocity_y(0);
		}
	}
}

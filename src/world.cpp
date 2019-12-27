#include <cmath>
#include <string>

#include "background.h"
#include "platform.h"
#include "sprite.h"
#include "spec.h"

#include "world.h"

World::World() : width(Spec::get_instance().get_int("world/width")), height(Spec::get_instance().get_int("world/height")), far(Spec::get_instance().get_float("world/far")),
		player(nullptr),
		owning{},
		drawables{},
		destroyables{},
		removables{} {}

World::~World() {
	// free added drawables
	for (Drawable * drawable : owning) {
		delete drawable;
	}
}

void World::init() {
	player = new Player();
	owning.insert(player);
	drawables.insert(player);

	// get top level elements and add applicable ones to drawables
	for (const std::string & str : Spec::get_instance().get_keys()) {
		if (Spec::get_instance().check(str + "/type")) {
			if (Spec::get_instance().get_str(str + "/type") == "background") {
				Background * background = new Background(str);
				owning.insert(background);
				drawables.insert(background);
			}
			else if (Spec::get_instance().get_str(str + "/type") == "platform") {
				Platform * platform = new Platform(str);
				owning.insert(platform);
				drawables.insert(platform);
			}
			else if (Spec::get_instance().get_str(str + "/type") == "body") {
				Body * body = new Body(str, Spec::get_instance().get_bool(str + "/fixed"));
				owning.insert(body);
				drawables.insert(body);
			}
			else if (Spec::get_instance().get_str(str + "/type") == "sprite") {
				Sprite * sprite = new Sprite(str);
				owning.insert(sprite);
				drawables.insert(sprite);
			}
		}
	}

	// load deferred drawables
	for (Drawable * drawable : drawables)
		drawable->load();
}

void World::destroy(Drawable * drawable) {
	destroyables.insert(drawable);
}

void World::add(Drawable & drawable) {
	drawables.insert(&drawable);
}

void World::remove(Drawable & drawable) {
	removables.insert(&drawable);
}

bool World::check(const Drawable * drawable) const {
	if (!drawable)
		return false;

	return drawables.find(const_cast<Drawable *>(drawable)) != drawables.end();
}

Drawable * World::get(const std::string & name) const {
	for (Drawable * drawable : drawables) {
		if (drawable->get_name() == name)
			return drawable;
	}

	for (Drawable * drawable : owning) {
		if (drawable->get_name() == name)
			return drawable;
	}

	return nullptr;
}

void World::dispatch(const SDL_Event & event) {
	// dispatch event to drawables
	for (Drawable * drawable : drawables)
		drawable->dispatch(event);
}

void World::update(unsigned int ticks) {
	// update drawables
	for (Drawable * drawable : drawables) {
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

		drawable->update(ticks, *this);
	}

	// remove deferred drawables
	for (Drawable * drawable : removables) {
		drawables.erase(drawable);
	}

	removables.clear();

	// destroy deferred drawables
	for (Drawable * drawable : destroyables) {
		drawables.erase(drawable);
		owning.erase(drawable);

		delete drawable;
	}

	destroyables.clear();
}

void World::draw(const Viewport & viewport) const {
	// draw ordered drawables
	for (Drawable * drawable : drawables)
		drawable->draw(viewport);
}

#include <cmath>
#include <string>

#include "background.h"
#include "platform.h"
#include "sprite.h"
#include "spec.h"

#include "world.h"

World::World() : _width(Spec::get_instance().get_int("world/width")), _height(Spec::get_instance().get_int("world/height")), _far(Spec::get_instance().get_float("world/far")),
		_player(nullptr),
		_owning{},
		_drawables{},
		_destroyables{},
		_removables{} {}

World::~World() {
	// free added drawables
	for (Drawable * drawable : _owning) {
		delete drawable;
	}
}

void World::init() {
	_player = new Player();
	_owning.insert(_player);
	_drawables.insert(_player);

	// get top level elements and add applicable ones to drawables
	for (const std::string & str : Spec::get_instance().get_keys()) {
		if (Spec::get_instance().check(str + "/type")) {
			if (Spec::get_instance().get_str(str + "/type") == "background") {
				Background * background = new Background(str);
				_owning.insert(background);
				_drawables.insert(background);
			}
			else if (Spec::get_instance().get_str(str + "/type") == "platform") {
				Platform * platform = new Platform(str);
				_owning.insert(platform);
				_drawables.insert(platform);
			}
			else if (Spec::get_instance().get_str(str + "/type") == "body") {
				Body * body = new Body(str, Spec::get_instance().get_bool(str + "/fixed"));
				_owning.insert(body);
				_drawables.insert(body);
			}
			else if (Spec::get_instance().get_str(str + "/type") == "sprite") {
				Sprite * sprite = new Sprite(str);
				_owning.insert(sprite);
				_drawables.insert(sprite);
			}
		}
	}

	// load deferred drawables
	for (Drawable * drawable : _drawables)
		drawable->load();
}

void World::destroy(Drawable * drawable) {
	_destroyables.insert(drawable);
}

void World::add(Drawable & drawable) {
	_drawables.insert(&drawable);
}

void World::remove(Drawable & drawable) {
	_removables.insert(&drawable);
}

bool World::check(const Drawable * drawable) const {
	if (!drawable)
		return false;

	return _drawables.find(const_cast<Drawable *>(drawable)) != _drawables.end();
}

Drawable * World::get(const std::string & name) const {
	for (Drawable * drawable : _drawables) {
		if (drawable->get_name() == name)
			return drawable;
	}

	for (Drawable * drawable : _owning) {
		if (drawable->get_name() == name)
			return drawable;
	}

	return nullptr;
}

void World::dispatch(const SDL_Event & event) {
	// dispatch event to drawables
	for (Drawable * drawable : _drawables)
		drawable->dispatch(event);
}

void World::update(unsigned int ticks) {
	// update drawables
	for (Drawable * drawable : _drawables) {
		// set position based on velocity delta
		Vector2f pos = drawable->get_position() + drawable->get_velocity()*ticks/1000;
		drawable->set_position(pos);

		// push away from edges
		if (drawable->get_x() < 0 && drawable->get_velocity_x() < 0) {
			drawable->set_x(0);
			drawable->set_velocity_x(0);
		}
		else if (drawable->get_x() > _width - drawable->get_width() && drawable->get_velocity_x() > 0) {
			drawable->set_x(_width - drawable->get_width());
			drawable->set_velocity_x(0);
		}

		if (drawable->get_y() < 0 && drawable->get_velocity_y() < 0) {
			drawable->set_y(0);
			drawable->set_velocity_y(0);
		}
		else if (drawable->get_y() > _height - drawable->get_height() && drawable->get_velocity_y() > 0) {
			drawable->set_y(_height - drawable->get_height());
			drawable->set_velocity_y(0);
		}

		drawable->update(ticks, *this);
	}

	// remove deferred drawables
	for (Drawable * drawable : _removables) {
		_drawables.erase(drawable);
	}

	_removables.clear();

	// destroy deferred drawables
	for (Drawable * drawable : _destroyables) {
		_drawables.erase(drawable);
		_owning.erase(drawable);

		delete drawable;
	}

	_destroyables.clear();
}

void World::draw(const Viewport & viewport) const {
	// draw ordered drawables
	for (Drawable * drawable : _drawables)
		drawable->draw(viewport);
}

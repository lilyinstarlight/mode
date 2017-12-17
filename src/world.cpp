#include <cmath>
#include <string>

#include "background.h"
#include "sprite.h"
#include "spec.h"

#include "world.h"

World::World() : width(Spec::get_instance().get_int("world/width")), height(Spec::get_instance().get_int("world/height")), far(Spec::get_instance().get_float("world/far")),
		player(nullptr),
		owning{},
		drawables{} {}

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
	for (const std::string & str : Spec::get_instance().get_tops()) {
		if (Spec::get_instance().check(str + "/type")) {
			if (Spec::get_instance().get_str(str + "/type") == "background") {
				Background * background = new Background(str);
				owning.insert(background);
				drawables.insert(background);
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
	drawables.erase(drawable);
	owning.erase(drawable);

	delete drawable;
}

void World::add(Drawable & drawable) {
	drawables.insert(&drawable);
}

void World::remove(Drawable & drawable) {
	drawables.erase(&drawable);
}

bool World::check(const Drawable * drawable) const {
	return drawables.find(const_cast<Drawable *>(drawable)) != drawables.end();
}

Drawable * World::get(const std::string & name) const {
	for (Drawable * drawable : drawables) {
		if (drawable->get_name() == name)
			return drawable;
	}

	return nullptr;
}

void World::dispatch(const SDL_Event & event) {
	// draw ordered drawables
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
}

void World::draw(const Viewport & viewport) const {
	// draw ordered drawables
	for (Drawable * drawable : drawables)
		drawable->draw(viewport);
}

std::pair<Drawable *, Vector2f> World::cast(Vector2f point, float direction) {
	Drawable * close = nullptr;
	Vector2f hit(point);
	float mag = far;

	for (Drawable * drawable : drawables) {
		std::vector<Vector2f> corners{Vector2f(drawable->get_x(), drawable->get_y()), Vector2f(drawable->get_x() + drawable->get_width(), drawable->get_y()), Vector2f(drawable->get_x(), drawable->get_y() + drawable->get_height()), Vector2f(drawable->get_x() + drawable->get_width(), drawable->get_y() + drawable->get_height())};

		for (Vector2f & corner : corners) {
			Vector2f cross(cos(direction)*(corner[1] - point[1]), sin(direction)*(corner[0] - point[0]));

			if (cross[0] > drawable->get_x() && cross[0] < drawable->get_x() + drawable->get_width()) {
				Vector2f pt(cross[0], drawable->get_y());
				float m = pt.magnitude();

				if (m < mag) {
					close = drawable;
					hit = pt;
					mag = m;
				}
			}

			if (cross[1] > drawable->get_y() && cross[1] < drawable->get_y() + drawable->get_height()) {
				Vector2f pt(drawable->get_x(), cross[1]);
				float m = pt.magnitude();

				if (m < mag) {
					close = drawable;
					hit = pt;
					mag = m;
				}
			}
		}
	}

	return std::make_pair(close, hit);
}

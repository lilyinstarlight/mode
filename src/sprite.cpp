#include <cmath>

#include "imagefactory.h"
#include "spec.h"

#include "sprite.h"

Sprite::Sprite(const std::string & name, const World & w, bool player) : Drawable(name,
			Vector2f(Spec::get_instance().get_int(name + "/position/x"),
			         Spec::get_instance().get_int(name + "/position/y")),
			rotation(Spec::get_instance().get_int(name + "/rotation")),
			Vector2f(Spec::get_instance().get_int(name + "/velocity/x"),
			         Spec::get_instance().get_int(name + "/velocity/y")),
			scale   (Spec::get_instance().get_int(name + "/scale"))
			),
		world(w),
		script(nullptr),
		sheet(ImageFactory::get_instance().get_sheet(name)),
		collision_strategy(&rectangular_strategy),
		frame(0),
		frames(Spec::get_instance().get_int(name + "/frames")),
		interval(Spec::get_instance().get_int(name + "/interval")),
		script_interval(500),
		frame_timer(0),
		script_timer(0) {
	if (!player)
		script = new Script(name, *this);

	switch (Spec::get_instance().get_str(name + "/collision")) {
		case "rectangular":
			collision_strategy = &rectangular_strategy;
			break;

		case "circular":
			collision_strategy = &circular_strategy;
			break;

		case "pixel":
			collision_strategy = &pixel_strategy;
			break;

		default:
			throw std::string("Invalid collision strategy: ") + Spec::get_instance().get_str(name + "/collision");
	}
}

Sprite::Sprite(const Sprite & s) :
		Drawable(s),
		script(new Script(s.script)),
		sheet(s.sheet),
		frame(s.frame),
		frames(s.frames),
		interval(s.interval),
		timer(s.timer) {
	if (s.collision_strategy == &s.rectangular_strategy)
		collision_strategy = &rectangular_strategy;
	else if (s.collision_strategy == &s.circular_strategy)
		collision_strategy = &circular_strategy;
	else if (s.collision_strategy == &s.pixel_strategy)
		collision_strategy = &pixel_strategy;
	else
		throw std::string("Invalid collision strategy while copying sprite: ") + get_name();
}

void Sprite::draw() const {
	get_image()->draw(get_x(), get_y(), get_scale());
}

void Sprite::update(unsigned int ticks) {
	frame_timer += ticks;
	if (frame_timer > interval) {
		frame = (frame + 1) % frames;
		frame_timer = 0;
	}

	script_timer += ticks;
	if (script_timer > interval) {
		script->call("update", ticks);
		script_timer = 0;
	}

	observer_timer += ticks;
	if (observer_timer > observer_interval) {
		for (const Observer & observer : observers)
			observer->signal("observe", *this);

		observer_timer = 0;
	}

	for (const Observer & observer : observers) {
		if (collision_strategy.check(*this, observer))
			observer->signal("collide", *this);
	}

	Vector2f delta = get_velocity()*static_cast<float>(ticks)*0.001;
	set_position(get_position() + delta);

	if (get_y() < 0)
	  set_velocity_y(std::abs(get_velocity_y()));
	else if (get_y() > world.get_height() - get_height()) {
	  set_velocity_y(-std::abs(get_velocity_y()));
	}

	if (get_x() < 0)
	  set_velocity_x(std::abs(get_velocity_y()));
	else if (get_x() > world.get_width() - get_width()) {
	  set_velocity_x(-std::abs(get_velocity_y()));
	}
}

void Sprite::observe(const Observer & observer) {
	observers.push_back(&observer);
}

void Sprite::ignore(const Observer & observer) {
	observers.remove(&observer);
}

#include <cmath>

#include "imagefactory.h"
#include "observer.h"
#include "spec.h"

#include "sprite.h"

Sprite::Sprite(const std::string & name, const World & w, bool own_script) : Drawable(name,
			Vector2f(Spec::get_instance().get_int(name + "/position/x"),
					 Spec::get_instance().get_int(name + "/position/y")),
					 Spec::get_instance().get_int(name + "/rotation"),
			Vector2f(Spec::get_instance().get_int(name + "/velocity/x"),
					 Spec::get_instance().get_int(name + "/velocity/y")),
					 Spec::get_instance().get_int(name + "/scale"),
					 Spec::get_instance().get_int(name + "/index")
		),
		world(w),
		script(nullptr),
		none_strategy(),
		rectangular_strategy(),
		circular_strategy(),
		pixel_strategy(),
		collision_strategy(&rectangular_strategy),
		observers{},
		sheets{},
		state("idle"),
		frame(0),
		script_interval(500),
		observer_interval(500),
		frame_timer(0),
		script_timer(0),
		observer_timer(0) {
	if (own_script)
		script = new Script(name, *this);

	for (const std::string & sheet : Spec::get_instance().get_subs(name + "/sheets"))
		sheets[sheet] = ImageFactory::get_instance().get_sheet(sheet);

	if (sheets.size() > 0)
		state = sheets.begin()->first;

	std::string collision = Spec::get_instance().get_str(name + "/collision");
	if (collision == "none")
		collision_strategy = &none_strategy;
	else if (collision == "rectangular")
		collision_strategy = &rectangular_strategy;
	else if (collision == "circular")
		collision_strategy = &circular_strategy;
	else if (collision == "pixel")
		collision_strategy = &pixel_strategy;
	else
		throw std::string("Invalid collision strategy: ") + Spec::get_instance().get_str(name + "/collision");
}

Sprite::Sprite(const Sprite & s) :
		Drawable(s),
		world(s.world),
		script(new Script(*s.script)),
		none_strategy(),
		rectangular_strategy(),
		circular_strategy(),
		pixel_strategy(),
		collision_strategy(&rectangular_strategy),
		observers(s.observers),
		sheets(s.sheets),
		state(s.state),
		frame(s.frame),
		script_interval(500),
		observer_interval(500),
		frame_timer(0),
		script_timer(0),
		observer_timer(0) {
	if (s.collision_strategy == &s.none_strategy)
		collision_strategy = &none_strategy;
	else if (s.collision_strategy == &s.rectangular_strategy)
		collision_strategy = &rectangular_strategy;
	else if (s.collision_strategy == &s.circular_strategy)
		collision_strategy = &circular_strategy;
	else if (s.collision_strategy == &s.pixel_strategy)
		collision_strategy = &pixel_strategy;
	else
		throw std::string("Invalid collision strategy while copying sprite: ") + get_name();
}

void Sprite::draw(const Viewport & viewport) const {
	get_image()->draw(viewport, get_x(), get_y(), get_scale());
}

void Sprite::update(unsigned int ticks) {
	frame_timer += ticks;
	if (sheets.at(state)->get_interval() > 0 && frame_timer > sheets.at(state)->get_interval()) {
		frame = (frame + 1) % sheets.at(state)->get_frames();
		frame_timer = 0;
	}

	script_timer += ticks;
	if (script_timer > script_interval) {
		script->call("update", ticks);
		script_timer = 0;
	}

	observer_timer += ticks;
	if (observer_timer > observer_interval) {
		for (Observer * observer : observers)
			observer->signal("observe", *this);

		observer_timer = 0;
	}

	for (Observer * observer : observers) {
		if (collision_strategy->check(*this, *observer))
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

void Sprite::observe(Observer & observer) {
	observers.push_back(&observer);
}

void Sprite::ignore(Observer & observer) {
	observers.remove(&observer);
}

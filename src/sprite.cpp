#include <cmath>

#include "imagefactory.h"
#include "spec.h"

#include "sprite.h"

Sprite::Sprite(const std::string & name) : Drawable(name,
			Vector2f(Spec::get_instance().get_int(name + "/position/x"),
					 Spec::get_instance().get_int(name + "/position/y")),
					 Spec::get_instance().get_int(name + "/rotation"),
			Vector2f(Spec::get_instance().get_int(name + "/velocity/x"),
					 Spec::get_instance().get_int(name + "/velocity/y")),
					 Spec::get_instance().get_int(name + "/scale"),
					 Spec::get_instance().get_int(name + "/index")
		),
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
		script_interval(200),
		observer_interval(200),
		frame_timer(0),
		script_timer(script_interval),
		observer_timer(observer_interval) {
	script = new Script(name, *this);

	// load sheets for different states
	for (const std::string & sheet : Spec::get_instance().get_subs(name + "/sheets"))
		sheets[sheet] = ImageFactory::get_instance().get_sheet(name + "/sheets/" + sheet);

	// set default as first sheet
	if (sheets.size() > 0)
		state = sheets.begin()->first;

	// set collision strategy
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
		throw std::runtime_error("Invalid collision strategy: " + Spec::get_instance().get_str(name + "/collision"));
}

Sprite::Sprite(const Sprite & s) :
		Drawable(s),
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
	// copy collision strategy
	if (s.collision_strategy == &s.none_strategy)
		collision_strategy = &none_strategy;
	else if (s.collision_strategy == &s.rectangular_strategy)
		collision_strategy = &rectangular_strategy;
	else if (s.collision_strategy == &s.circular_strategy)
		collision_strategy = &circular_strategy;
	else if (s.collision_strategy == &s.pixel_strategy)
		collision_strategy = &pixel_strategy;
	else
		throw std::runtime_error("Invalid collision strategy while copying sprite: " + get_name());
}

Sprite::~Sprite() {
	delete script;
}

void Sprite::draw(const Viewport & viewport) const {
	// draw current image in viewport
	get_image()->draw(viewport, get_x(), get_y(), get_scale());
}

void Sprite::update(unsigned int ticks) {
	// increment frame as necessary
	frame_timer += ticks;
	if (sheets.at(state)->get_interval() > 0 && frame_timer > sheets.at(state)->get_interval()) {
		frame = (frame + 1) % sheets.at(state)->get_frames();
		frame_timer = 0;
	}

	// run script update as necessary
	script_timer += ticks;
	if (script_timer > script_interval) {
		script->call("update", script_timer);
		script_timer = 0;
	}

	// run script obverse for observers as necessary
	observer_timer += ticks;
	if (observer_timer > observer_interval) {
		for (Sprite * observer : observers)
			observer->signal("observe", *this);

		observer_timer = 0;
	}

	// check collision with observers
	for (Sprite * observer : observers) {
		if (collision_strategy->check(*this, *observer))
			observer->signal("collide", *this);
	}
}

const Image * Sprite::get_image() const {
	return sheets.at(state)->get_image(frame);
}

int Sprite::get_width() const {
	return get_scale()*sheets.at(state)->get_image(frame)->get_width();
}

int Sprite::get_height() const {
	return get_scale()*sheets.at(state)->get_image(frame)->get_height();
}

const SDL_Surface * Sprite::get_surface() const {
	return sheets.at(state)->get_image(frame)->get_surface();
}

void Sprite::observe(Sprite & observer) {
	// add observer
	observers.push_back(&observer);
}

void Sprite::ignore(Sprite & observer) {
	// remove observer
	observers.remove(&observer);
}

void Sprite::signal(const std::string & sig, const Sprite & sprite) {
	// call script with signal
	script->call(sig, sprite);
}

void Sprite::inject() {
	// TODO: pull up script of object script in HUD
}

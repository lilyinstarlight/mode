#include <SDL2/SDL2_rotozoom.h>

#include "imagefactory.h"
#include "spec.h"
#include "util.h"

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
		direction(Spec::get_instance().get_str(name + "/direction")),
		state(),
		observer_interval(200),
		frame_timer(0),
		observer_timer(observer_interval) {
	script = new Script(name, *this);

	state.push_back(std::pair<std::string, unsigned int>{Spec::get_instance().get_str(name + "/state"), 0});

	// load sheets for different states
	for (const std::string & sheet : Spec::get_instance().get_subs(name + "/sheets")) {
		for (const std::string & direction : Spec::get_instance().get_subs(name + "/sheets/" + sheet))
			sheets[sheet + "." + direction] = ImageFactory::get_instance().get_sheet(name + "/sheets/" + sheet + "/" + direction);
	}

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
		throw std::runtime_error("Invalid collision strategy: " + Spec::get_instance().get_str(get_name() + "/collision"));
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
		direction(s.direction),
		state(s.state),
		observer_interval(s.observer_interval),
		frame_timer(s.frame_timer),
		observer_timer(s.observer_timer) {
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

void Sprite::load() {
	script->load();
}

void Sprite::dispatch(const SDL_Event & event) {
	// run script dispatch as necessary
	script->call("dispatch", &event);
}

void Sprite::draw(const Viewport & viewport) const {
	// draw current image in viewport
	get_image()->draw(viewport, get_x(), get_y(), get_rotation(), get_scale());
}

void Sprite::update(unsigned int ticks) {
	if (get_velocity_x() < 0)
		direction = "left";
	else if (get_velocity_x() > 0)
		direction = "right";

	// run script update as necessary
	script->call("update", ticks);

	// increment frame as necessary
	Sheet * sheet = sheets.at(state.back().first + "." + direction);
	frame_timer += ticks;
	if (sheet->get_interval() > 0 && frame_timer > sheet->get_interval()) {
		state.back().second += 1;

		if (state.back().second >= static_cast<unsigned int>(sheet->get_frames())) {
			if (sheet->get_loop())
				state.back().second = 0;
			else {
				if (state.size() > 0)
					state.pop_back();
				else
					state.back().second = 0;
			}
		}

		frame_timer = 0;
	}

	// run script obverse for observers as necessary
	observer_timer += ticks;
	if (observer_timer > observer_interval) {
		for (Sprite * observer : observers)
			observer->send("observe", *this);

		observer_timer = 0;
	}

	// check collision with observers
	for (Sprite * observer : observers) {
		if (collision_strategy->check(*this, *observer))
			observer->send("collide", *this);
	}
}

const Image * Sprite::get_image() const {
	return sheets.at(state.back().first + "." + direction)->get_image(state.back().second);
}

int Sprite::get_width() const {
	const Image * image = sheets.at(state.back().first + "." + direction)->get_image(state.back().second);

	int width, height;
	if (get_rotation() <= Vector2f::EPSILON)
		width = image->get_width()*get_scale();
	else
		rotozoomSurfaceSize(image->get_width(), image->get_height(), -get_rotation(), get_scale(), &width, &height);

	return width;
}

int Sprite::get_height() const {
	const Image * image = sheets.at(state.back().first + "." + direction)->get_image(state.back().second);

	int width, height;
	if (get_rotation() <= Vector2f::EPSILON)
		height = image->get_height()*get_scale();
	else
		rotozoomSurfaceSize(image->get_width(), image->get_height(), -get_rotation(), get_scale(), &width, &height);

	return height;
}

const SDL_Surface * Sprite::get_surface() const {
	const Image * image = sheets.at(state.back().first + "." + direction)->get_image(state.back().second);
	return image->get_surface();
}

std::string Sprite::get_state() const {
	return state.front().first;
}

void Sprite::set_state(const std::string & s) {
	if (s == state.front().first)
		return;

	state.front() = std::pair<std::string, unsigned int>{s, 0};
	frame_timer = 0;
}

std::string Sprite::peek_state() const {
	return state.back().first;
}

std::string Sprite::pop_state() {
	const std::string & s = state.back().first;
	state.pop_back();

	return s;
}

void Sprite::clear_state() {
	while (state.size() > 1)
		state.pop_back();
}

void Sprite::push_state(const std::string & s) {
	if (s == state.back().first)
		return;

	state.push_back(std::pair<std::string, unsigned int>{s, 0});
	frame_timer = 0;
}

void Sprite::observe(Sprite & observer) {
	// add observer
	observers.push_back(&observer);
}

void Sprite::ignore(Sprite & observer) {
	// remove observer
	observers.remove(&observer);
}

void Sprite::signal(const std::string & sig) {
	// call script with signal
	script->call(sig);
}

void Sprite::send(const std::string & sig, Sprite & sprite) {
	// call script with signal and sprite
	script->call(sig, &sprite);
}

void Sprite::inject() {
	// TODO: pull up script of object script in HUD
}

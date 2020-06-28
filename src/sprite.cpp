#include <stdexcept>

#include <SDL2_rotozoom.h>

#include "engine.h"
#include "imagefactory.h"
#include "sound.h"
#include "spec.h"
#include "util.h"
#include "world.h"

#include "sprite.h"

Sprite::Sprite(const std::string & name, bool dead) : Drawable(name,
			Spec::get_instance().get_str(name + "/type"),
			Vector2f(Spec::get_instance().get_int(name + "/position/x"),
					 Spec::get_instance().get_int(name + "/position/y")),
			Spec::get_instance().get_int(name + "/rotation"),
			Vector2f(Spec::get_instance().get_int(name + "/velocity/x"),
					 Spec::get_instance().get_int(name + "/velocity/y")),
			Spec::get_instance().get_int(name + "/scale"),
			Spec::get_instance().get_int(name + "/index")
		),
		_script(nullptr),
		_none_strategy(),
		_rectangular_strategy(),
		_circular_strategy(),
		_pixel_strategy(),
		_collision_strategy(&_rectangular_strategy),
		_observers{},
		_sheets{},
		_direction(Spec::get_instance().get_str(name + "/direction")),
		_state(),
		_alive(!dead),
		_observer_interval(200),
		_frame_timer(0),
		_observer_timer(_observer_interval) {
	_script = new Script(name, *this);

	_state.push_back(std::pair<std::string, unsigned int>{Spec::get_instance().get_str(name + "/state"), 0});

	// load sheets for different states
	for (const std::string & sheet : Spec::get_instance().get_keys(name + "/sheets")) {
		for (const std::string & sheet_direction : Spec::get_instance().get_keys(name + "/sheets/" + sheet))
			_sheets[sheet + "." + sheet_direction] = ImageFactory::get_instance().get_sheet(name + "/sheets/" + sheet + "/" + sheet_direction);
	}

	// set collision strategy
	std::string collision = Spec::get_instance().get_str(name + "/collision");
	if (collision == "none")
		_collision_strategy = &_none_strategy;
	else if (collision == "rectangular")
		_collision_strategy = &_rectangular_strategy;
	else if (collision == "circular")
		_collision_strategy = &_circular_strategy;
	else if (collision == "pixel")
		_collision_strategy = &_pixel_strategy;
	else
		throw std::runtime_error("Invalid collision strategy: " + Spec::get_instance().get_str(get_name() + "/collision"));
}

Sprite::Sprite(const Sprite & s) :
		Drawable(s),
		_script(new Script(*s._script)),
		_none_strategy(),
		_rectangular_strategy(),
		_circular_strategy(),
		_pixel_strategy(),
		_collision_strategy(&_rectangular_strategy),
		_observers(s._observers),
		_sheets(s._sheets),
		_direction(s._direction),
		_state(s._state),
		_alive(s._alive),
		_observer_interval(s._observer_interval),
		_frame_timer(s._frame_timer),
		_observer_timer(s._observer_timer) {
	// copy collision strategy
	if (s._collision_strategy == &s._none_strategy)
		_collision_strategy = &_none_strategy;
	else if (s._collision_strategy == &s._rectangular_strategy)
		_collision_strategy = &_rectangular_strategy;
	else if (s._collision_strategy == &s._circular_strategy)
		_collision_strategy = &_circular_strategy;
	else if (s._collision_strategy == &s._pixel_strategy)
		_collision_strategy = &_pixel_strategy;
	else
		throw std::runtime_error("Invalid collision strategy while copying sprite: " + get_name());
}

Sprite::~Sprite() {
	delete _script;
}

void Sprite::load() {
	_script->load();
}

void Sprite::dispatch(const SDL_Event & event) {
	// run script dispatch as necessary
	_script->signal("dispatch", &event);
}

void Sprite::draw(const Viewport & viewport) const {
	// draw current image in viewport
	get_image()->draw(viewport, get_x(), get_y(), get_rotation(), get_scale());
}

void Sprite::update(unsigned int ticks, World & world) {
	if (get_velocity_x() < 0)
		_direction = "left";
	else if (get_velocity_x() > 0)
		_direction = "right";

	// run script update as necessary
	_script->signal("update", ticks);

	// increment frame as necessary
	Sheet * sheet = _sheets.at(_state.back().first + "." + _direction);
	_frame_timer += ticks;
	if (sheet->get_interval() > 0 && _frame_timer > sheet->get_interval()) {
		_state.back().second += 1;

		if (_state.back().second >= sheet->get_frames()) {
			if (sheet->get_loop()) {
				_state.back().second = 0;
			}
			else {
				if (_state.size() > 0)
					_state.pop_back();
				else
					_state.back().second = 0;
			}

			if (!_alive) {
				world.remove(*this);
				return;
			}
		}

		_frame_timer = 0;
	}

	// run script observe for observers as necessary
	_observer_timer += ticks;
	if (_observer_timer > _observer_interval) {
		for (Sprite * observer : _observers)
			observer->send("observe", *this);

		_observer_timer = 0;
	}

	// check collision with observers
	for (Sprite * observer : _observers) {
		if (_collision_strategy->check(*this, *observer))
			observer->send("collide", *this);
	}
}

void Sprite::revive() {
	if (_alive)
		return;

	_alive = true;
	clear_state();

	if (Spec::get_instance().check(get_name() + "/sheets/revive"))
		push_state("revive");

	if (Spec::get_instance().check(get_name() + "/sound/revive"))
		Sound::get_instance().play(get_name() + "/sound/revive");

	signal("revive");
}

void Sprite::kill() {
	if (!_alive)
		return;

	_alive = false;
	clear_state();

	if (Spec::get_instance().check(get_name() + "/sheets/kill"))
		push_state("kill");

	if (Spec::get_instance().check(get_name() + "/sound/kill"))
		Sound::get_instance().play(get_name() + "/sound/kill");

	signal("kill");
}

const Image * Sprite::get_image() const {
	return _sheets.at(_state.back().first + "." + _direction)->get_image(_state.back().second);
}

int Sprite::get_width() const {
	const Image * image = _sheets.at(_state.back().first + "." + _direction)->get_image(_state.back().second);

	int width, height;
	if (get_rotation() <= Vector2f::EPSILON)
		width = image->get_width()*get_scale();
	else
		rotozoomSurfaceSize(image->get_width(), image->get_height(), -get_rotation(), get_scale(), &width, &height);

	return width;
}

int Sprite::get_height() const {
	const Image * image = _sheets.at(_state.back().first + "." + _direction)->get_image(_state.back().second);

	int width, height;
	if (get_rotation() <= Vector2f::EPSILON)
		height = image->get_height()*get_scale();
	else
		rotozoomSurfaceSize(image->get_width(), image->get_height(), -get_rotation(), get_scale(), &width, &height);

	return height;
}

const SDL_Surface * Sprite::get_surface() const {
	const Image * image = _sheets.at(_state.back().first + "." + _direction)->get_image(_state.back().second);
	return image->get_surface();
}

std::string Sprite::get_state() const {
	return _state.front().first;
}

void Sprite::set_state(const std::string & state) {
	if (state == _state.front().first)
		return;

	_state.front() = std::pair<std::string, unsigned int>{state, 0};
	_frame_timer = 0;
}

std::string Sprite::peek_state() const {
	return _state.back().first;
}

std::string Sprite::pop_state() {
	if (_state.back().first == "revive" || _state.back().first == "kill")
		return _state.back().first;

	const std::string & state = _state.back().first;
	_state.pop_back();

	return state;
}

void Sprite::clear_state() {
	if (_state.back().first == "revive" || _state.back().first == "kill")
		return;

	while (_state.size() > 1)
		_state.pop_back();
}

void Sprite::push_state(const std::string & state) {
	if (_state.back().first == "revive" || _state.back().first == "kill")
		return;

	if (state == _state.back().first)
		return;

	_state.push_back(std::pair<std::string, unsigned int>{state, 0});
	_frame_timer = 0;
}

std::vector<std::string> Sprite::get_full_state() const {
	std::vector<std::string> full;
	full.reserve(_state.size());

	for (const std::pair<std::string, unsigned int> & state : _state)
		full.push_back(state.first);

	return full;
}

void Sprite::observe(Sprite & observer) {
	// add observer
	_observers.push_back(&observer);
}

void Sprite::ignore(Sprite & observer) {
	// remove observer
	_observers.remove(&observer);
}

void Sprite::signal(const std::string & sig) {
	// call script with signal
	_script->signal(sig);
}

void Sprite::send(const std::string & sig, Sprite & sprite) {
	// call script with signal and sprite
	_script->signal(sig, &sprite);
}

void Sprite::inject() {
	// pull up editor for script
	Engine::get_instance().get_editor().open(*_script);
}

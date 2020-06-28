#include <fstream>
#include <stdexcept>

#include "background.h"
#include "clock.h"
#include "collision.h"
#include "engine.h"
#include "hud.h"
#include "platform.h"
#include "player.h"
#include "projectile.h"
#include "save.h"
#include "sound.h"
#include "spec.h"
#include "sprite.h"
#include "vector2f.h"
#include "viewport.h"
#include "world.h"

#include "script.h"

Script::Script() : _path("behaviours"), _interactive(true), _script(""), _lua(), _sprite(Engine::get_instance().get_world().get_player()) {}

Script::Script(const std::string & name, Sprite & sprite) : _path("behaviours"), _interactive(false), _script(""), _lua(), _sprite(sprite) {
	// load file
	load_file(_path + "/" + name + ".lua");
}

Script::Script(const Script & s) : _path(s._path), _interactive(s._interactive), _script(s._script), _lua(), _sprite(s._sprite) {}

void Script::load() {
	// prepare environment
	load_api();

	if (_interactive)
		_lua.require_file("repl", "libs/repl.lua");
	else
		_lua.script(_script);
}

void Script::set_script(const std::string & script) {
	// refresh state
	_lua = sol::state();

	// set new script
	_script = script;

	// reload
	load();
}

std::tuple<int, std::string> Script::repl(const std::string & script) {
	if (!_interactive)
		return std::make_tuple<int, std::string>(-1, "error: non-interactive interpreter");

	try {
		int status;
		sol::object result;

		// repl line
		sol::tie(status, result) = _lua["repl"](script);

		std::string str;
		if (result.get_type() == sol::type::lua_nil)
			str = "";
		else
			str = result.as<std::string>();

		return std::make_tuple(status, str);
	}
	catch (sol::error & err) {
		// return internal error not caught by REPL
		return std::make_tuple<int, std::string>(-1, "internal error: " + std::string(err.what()));
	}
}

void Script::load_api() {
	if (_interactive)
		_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);
	else
		_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::math, sol::lib::table);

	// create Vector data type
	_lua.new_usertype<Vector2f>("Vector",
			"new", sol::constructors<Vector2f(float, float)>(),

			"x", sol::property(&Vector2f::get_x, &Vector2f::set_x),
			"y", sol::property(&Vector2f::get_y, &Vector2f::set_y)
	);

	// create Sprite data type
	_lua.new_usertype<Sprite>("Sprite",
			"new", sol::factories(WrapCreate<World, Sprite, std::string>(Engine::get_instance().get_world())),

			"observe", WrapObserve<Sprite, Sprite>(_sprite),
			"ignore", WrapIgnore<Sprite, Sprite>(_sprite),

			"signal", &Sprite::signal,
			"send", &Sprite::send,

			"revive", &Sprite::revive,
			"kill", &Sprite::kill,

			"inject", &Sprite::inject,

			"direction", sol::property(&Sprite::get_direction, &Sprite::set_direction),
			"state", sol::property(&Sprite::get_state, &Sprite::set_state),
			"peek", &Sprite::peek_state,
			"pop", &Sprite::pop_state,
			"clear", &Sprite::clear_state,
			"push", &Sprite::push_state,
			"get_state", &Sprite::get_full_state,

			"name", sol::property(&Sprite::get_name),
			"type", sol::property(&Sprite::get_type),

			"width", sol::property(&Sprite::get_width),
			"height", sol::property(&Sprite::get_height),

			"pos", sol::property(&Sprite::get_position, &Sprite::set_position),
			"rot", sol::property(&Sprite::get_rotation, &Sprite::set_rotation),
			"vel", sol::property(&Sprite::get_velocity, &Sprite::set_velocity),
			"scale", sol::property(&Sprite::get_scale, &Sprite::set_scale),
			"idx", sol::property(&Sprite::get_index, &Sprite::set_index)
	);

	// set sprite as current sprite
	_lua["sprite"] = &_sprite;

	// create Sprite data type
	_lua.new_usertype<Body>("Body",
			"new", sol::factories(WrapCreate<World, Body, std::string, bool>(Engine::get_instance().get_world())),

			"name", sol::property(&Body::get_name),

			"hardness", sol::property(&Body::get_hardness, &Body::set_hardness),
			"elasticity", sol::property(&Body::get_elasticity, &Body::set_elasticity),

			"fixed", sol::property(&Body::is_fixed, &Body::set_fixed)
	);

	// set body as current sprite if a body
	_lua["body"] = dynamic_cast<Body *>(&_sprite);

	// create Sprite data type
	_lua.new_usertype<Platform>("Platform",
			"new", sol::factories(WrapCreate<World, Platform, std::string>(Engine::get_instance().get_world())),

			"name", sol::property(&Platform::get_name),

			"tile", sol::property(&Platform::get_tile)
	);

	// set platform as current sprite if a platform
	_lua["platform"] = dynamic_cast<Platform *>(&_sprite);

	// create Projectile data type
	_lua.new_usertype<Projectile>("Projectile",
			"new", sol::factories(WrapCreate<World, Projectile, std::string>(Engine::get_instance().get_world())),

			"name", sol::property(&Projectile::get_name),

			"alive", sol::property(&Projectile::is_alive),
			"kill", &Projectile::kill,
			"origin", sol::property(&Projectile::get_origin)
	);

	// set projectile as current sprite if a projectile
	_lua["projectile"] = dynamic_cast<Projectile *>(&_sprite);

	// create Player data type
	_lua.new_usertype<Player>("Player",
			"new", sol::no_constructor,

			"name", sol::property(&Player::get_name),

			"observe", WrapObserve<Player, Sprite>(_sprite),
			"ignore", WrapIgnore<Player, Sprite>(_sprite),

			"shoot", &Player::shoot
	);

	// set player as current player
	Player * player = dynamic_cast<Player *>(&_sprite);
	if (!player)
		player = &Engine::get_instance().get_world().get_player();

	_lua["player"] = player;

	// create Background data type
	_lua.new_usertype<Background>("Background",
			"new", sol::factories(WrapCreate<World, Background, std::string>(Engine::get_instance().get_world())),

			"factor", sol::property(&Background::get_factor, &Background::set_factor),

			"name", sol::property(&Background::get_name),

			"width", sol::property(&Background::get_width),
			"height", sol::property(&Background::get_height),

			"pos", sol::property(&Background::get_position, &Background::set_position),
			"rot", sol::property(&Background::get_rotation, &Background::set_rotation),
			"vel", sol::property(&Background::get_velocity, &Background::set_velocity),
			"scale", sol::property(&Background::get_scale, &Background::set_scale),
			"idx", sol::property(&Background::get_index, &Background::set_index),

			"tile", sol::property(&Background::get_tile)
	);

	// create World data type
	_lua.new_usertype<World>("World",
			"new", sol::no_constructor,

			"add_sprite", WrapAdd<World, Sprite>(),
			"remove_sprite", WrapRemove<World, Sprite>(),
			"get_sprite", WrapGet<World, Sprite>(),
			"check_sprite", WrapCheck<World, Sprite>(),
			"destroy_sprite", WrapDestroy<World, Sprite>(),

			"add_platform", WrapAdd<World, Platform>(),
			"remove_platform", WrapRemove<World, Platform>(),
			"get_platform", WrapGet<World, Platform>(),
			"check_platform", WrapCheck<World, Platform>(),
			"destroy_platform", WrapDestroy<World, Platform>(),

			"add_background", WrapAdd<World, Background>(),
			"remove_background", WrapRemove<World, Background>(),
			"get_background", WrapGet<World, Background>(),
			"check_background", WrapCheck<World, Background>(),
			"destroy_background", WrapDestroy<World, Background>(),

			"add_dialog", WrapAdd<World, Dialog>(),
			"remove_dialog", WrapRemove<World, Dialog>(),
			"get_dialog", WrapGet<World, Dialog>(),
			"check_dialog", WrapCheck<World, Dialog>(),
			"destroy_dialog", WrapDestroy<World, Dialog>(),

			"add_body", WrapAdd<World, Body>(),
			"remove_body", WrapRemove<World, Body>(),
			"get_body", WrapGet<World, Body>(),
			"check_body", WrapCheck<World, Body>(),
			"destroy_body", WrapDestroy<World, Body>(),

			"add_platform", WrapAdd<World, Platform>(),
			"remove_platform", WrapRemove<World, Platform>(),
			"get_platform", WrapGet<World, Platform>(),
			"check_platform", WrapCheck<World, Platform>(),
			"destroy_platform", WrapDestroy<World, Platform>(),

			"cast", &World::cast<Sprite>,

			"width", sol::property(&World::get_width),
			"height", sol::property(&World::get_height),

			"far", sol::property(&World::get_far)
	);

	// set world as current world
	_lua["world"] = &Engine::get_instance().get_world();

	// create Input data type
	_lua.new_usertype<Input>("Input",
			"new", sol::no_constructor,

			"grab", &Input::grab,
			"release", &Input::release,
			"check", &Input::check,
			"get", &Input::get,

			"get_list", &Input::get_list,

			"get_key", WrapGetKey<Input>(_lua)
	);

	// set input
	_lua["input"] = &Input::get_instance();

	// create Spec data type
	_lua.new_usertype<Spec>("Spec",
			"new", sol::no_constructor,

			"check", &Spec::check,

			"get_keys", &Spec::get_keys,

			"get_bool", &Spec::get_bool,
			"get_int", &Spec::get_int,
			"get_float", &Spec::get_float,
			"get_str", &Spec::get_str
	);

	// set spec
	_lua["spec"] = &Spec::get_instance();

	// create Save data type
	_lua.new_usertype<Save>("Save",
			"new", sol::no_constructor,

			"check", &Save::check,

			"get_keys", &Save::get_keys,

			"get_bool", &Save::get_bool,
			"get_int", &Save::get_int,
			"get_float", &Save::get_float,
			"get_str", &Save::get_str,

			"remove", &Save::remove,

			"set_bool", &Save::set_bool,
			"set_int", &Save::set_int,
			"set_float", &Save::set_float,
			"set_str", &Save::set_str,

			"write", &Save::write
	);

	// set spec
	_lua["save"] = &Save::get_instance();

	// create Engine data type
	_lua.new_usertype<Engine>("Engine",
			"new", sol::no_constructor,

			"load", &Engine::load,

			"restart", &Engine::restart,
			"stop", &Engine::stop
	);

	// set engine
	_lua["engine"] = &Engine::get_instance();

	// create Clock data type
	_lua.new_usertype<Clock>("Clock",
			"new", sol::no_constructor,

			"ticks", sol::property(&Clock::get_ticks),
			"fps", sol::property(&Clock::get_fps)
	);

	// set clock
	_lua["clock"] = &Clock::get_instance();

	// create Sound data type
	_lua.new_usertype<Sound>("Sound",
			"new", sol::no_constructor,

			"active", sol::property(&Sound::check),

			"play", &Sound::play
	);

	// set sound
	_lua["sound"] = &Sound::get_instance();

	// create Dialog data type
	_lua.new_usertype<Dialog>("Dialog",
			"new", sol::factories(WrapCreate<World, Dialog, std::string, std::string>(Engine::get_instance().get_world()), WrapCreate<World, Dialog, std::string, std::string, bool>(Engine::get_instance().get_world()), WrapCreate<World, Dialog, std::string, std::string, bool, bool>(Engine::get_instance().get_world())),

			"name", sol::property(&Dialog::get_name),

			"open", &Dialog::open,
			"close", &Dialog::close,
			"toggle", &Dialog::toggle,

			"opened", sol::property(&Dialog::is_open),

			"string", sol::property(&Dialog::get_string, &Dialog::set_string),

			"pos", sol::property(&Dialog::get_position, &Dialog::set_position),
			"rot", sol::property(&Dialog::get_rotation, &Dialog::set_rotation),
			"vel", sol::property(&Dialog::get_velocity, &Dialog::set_velocity),
			"scale", sol::property(&Dialog::get_scale, &Dialog::set_scale),
			"idx", sol::property(&Dialog::get_index, &Dialog::set_index)
	);

	// create HUD data type
	_lua.new_usertype<HUD>("HUD",
			"new", sol::no_constructor,

			"open", &HUD::open,
			"close", &HUD::close,
			"toggle", &HUD::toggle,

			"opened", sol::property(&HUD::is_open),

			"string", sol::property(&HUD::get_string, &HUD::set_string)
	);

	// set hud
	_lua["hud"] = &Engine::get_instance().get_hud();

	// create Viewport data type
	_lua.new_usertype<Viewport>("Viewport",
			"new", sol::no_constructor,

			"width", sol::property(&Viewport::get_width),
			"height", sol::property(&Viewport::get_height),

			"pos", sol::property(&Viewport::get_position, &Viewport::set_position),

			"tracking", sol::property(&Viewport::get_tracking, &Viewport::track)
	);

	// set viewport
	_lua["view"] = &Engine::get_instance().get_viewport();

	// create Event data type
	_lua.new_usertype<SDL_Event>("Event",
			"new", sol::no_constructor,

			"ev", sol::property(WrapEventType(_lua)),
			"val", sol::property(WrapEventValue(_lua))
	);
}

void Script::load_file(const std::string & filename) {
	// load file contents
	std::ifstream file(filename);
	if (!file)
		throw std::runtime_error("Failed to load script " + filename);

	_script = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

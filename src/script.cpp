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
			"get_pressed", &Input::get_pressed,

			"get_key", WrapGetScancode<Input>(&Input::get_key),
			"check_key", WrapGetScancode<Input>(&Input::check_key)
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

const std::unordered_map<SDL_Scancode, std::string> Script::sdl_codes = {
	{SDL_SCANCODE_0, "0"},
	{SDL_SCANCODE_1, "1"},
	{SDL_SCANCODE_2, "2"},
	{SDL_SCANCODE_3, "3"},
	{SDL_SCANCODE_4, "4"},
	{SDL_SCANCODE_5, "5"},
	{SDL_SCANCODE_6, "6"},
	{SDL_SCANCODE_7, "7"},
	{SDL_SCANCODE_8, "8"},
	{SDL_SCANCODE_9, "9"},

	{SDL_SCANCODE_A, "a"},
	{SDL_SCANCODE_B, "b"},
	{SDL_SCANCODE_C, "c"},
	{SDL_SCANCODE_D, "d"},
	{SDL_SCANCODE_E, "e"},
	{SDL_SCANCODE_F, "f"},
	{SDL_SCANCODE_G, "g"},
	{SDL_SCANCODE_H, "h"},
	{SDL_SCANCODE_I, "i"},
	{SDL_SCANCODE_J, "j"},
	{SDL_SCANCODE_K, "k"},
	{SDL_SCANCODE_L, "l"},
	{SDL_SCANCODE_M, "m"},
	{SDL_SCANCODE_N, "n"},
	{SDL_SCANCODE_O, "o"},
	{SDL_SCANCODE_P, "p"},
	{SDL_SCANCODE_Q, "q"},
	{SDL_SCANCODE_R, "r"},
	{SDL_SCANCODE_S, "s"},
	{SDL_SCANCODE_T, "t"},
	{SDL_SCANCODE_U, "u"},
	{SDL_SCANCODE_V, "v"},
	{SDL_SCANCODE_W, "w"},
	{SDL_SCANCODE_X, "x"},
	{SDL_SCANCODE_Y, "y"},
	{SDL_SCANCODE_Z, "z"},

	{SDL_SCANCODE_LEFT, "left"},
	{SDL_SCANCODE_RIGHT, "right"},
	{SDL_SCANCODE_UP, "up"},
	{SDL_SCANCODE_DOWN, "down"},

	{SDL_SCANCODE_F1, "f1"},
	{SDL_SCANCODE_F2, "f2"},
	{SDL_SCANCODE_F3, "f3"},
	{SDL_SCANCODE_F4, "f4"},
	{SDL_SCANCODE_F5, "f5"},
	{SDL_SCANCODE_F6, "f6"},
	{SDL_SCANCODE_F7, "f7"},
	{SDL_SCANCODE_F8, "f8"},
	{SDL_SCANCODE_F9, "f9"},
	{SDL_SCANCODE_F10, "f10"},
	{SDL_SCANCODE_F11, "f11"},
	{SDL_SCANCODE_F12, "f12"},
	{SDL_SCANCODE_F13, "f13"},
	{SDL_SCANCODE_F14, "f14"},
	{SDL_SCANCODE_F15, "f15"},
	{SDL_SCANCODE_F16, "f16"},
	{SDL_SCANCODE_F17, "f17"},
	{SDL_SCANCODE_F18, "f18"},
	{SDL_SCANCODE_F19, "f19"},
	{SDL_SCANCODE_F20, "f20"},
	{SDL_SCANCODE_F21, "f21"},
	{SDL_SCANCODE_F22, "f22"},
	{SDL_SCANCODE_F23, "f23"},
	{SDL_SCANCODE_F24, "f24"},

	{SDL_SCANCODE_APOSTROPHE, "'"},
	{SDL_SCANCODE_BACKSLASH, "\\"},
	{SDL_SCANCODE_COMMA, ","},
	{SDL_SCANCODE_EQUALS, "="},
	{SDL_SCANCODE_GRAVE, "`"},
	{SDL_SCANCODE_LEFTBRACKET, "["},
	{SDL_SCANCODE_MINUS, "-"},
	{SDL_SCANCODE_PERIOD, "."},
	{SDL_SCANCODE_RIGHTBRACKET, "]"},
	{SDL_SCANCODE_SEMICOLON, ";"},
	{SDL_SCANCODE_SLASH, "/"},
	{SDL_SCANCODE_SPACE, " "},

	{SDL_SCANCODE_BACKSPACE, "back"},
	{SDL_SCANCODE_DELETE, "delete"},
	{SDL_SCANCODE_END, "end"},
	{SDL_SCANCODE_HOME, "home"},
	{SDL_SCANCODE_INSERT, "insert"},

	{SDL_SCANCODE_PAGEDOWN, "pgdown"},
	{SDL_SCANCODE_PAGEUP, "pgup"},

	{SDL_SCANCODE_RETURN, "return"},
	{SDL_SCANCODE_TAB, "tab"},

	{SDL_SCANCODE_PRINTSCREEN, "prntscrn"},

	{SDL_SCANCODE_CAPSLOCK, "caps"},
	{SDL_SCANCODE_ESCAPE, "esc"},
	{SDL_SCANCODE_NUMLOCKCLEAR, "num"},
	{SDL_SCANCODE_SCROLLLOCK, "scroll"},

	{SDL_SCANCODE_LALT, "lalt"},
	{SDL_SCANCODE_LCTRL, "lctrl"},
	{SDL_SCANCODE_LSHIFT, "lshift"},
	{SDL_SCANCODE_RALT, "ralt"},
	{SDL_SCANCODE_RCTRL, "rctrl"},
	{SDL_SCANCODE_RSHIFT, "rshift"},

	{SDL_SCANCODE_MENU, "menu"}
};

const std::unordered_map<std::string, SDL_Scancode> Script::sdl_rev_codes = {
	{"0", SDL_SCANCODE_0},
	{"1", SDL_SCANCODE_1},
	{"2", SDL_SCANCODE_2},
	{"3", SDL_SCANCODE_3},
	{"4", SDL_SCANCODE_4},
	{"5", SDL_SCANCODE_5},
	{"6", SDL_SCANCODE_6},
	{"7", SDL_SCANCODE_7},
	{"8", SDL_SCANCODE_8},
	{"9", SDL_SCANCODE_9},

	{"a", SDL_SCANCODE_A},
	{"b", SDL_SCANCODE_B},
	{"c", SDL_SCANCODE_C},
	{"d", SDL_SCANCODE_D},
	{"e", SDL_SCANCODE_E},
	{"f", SDL_SCANCODE_F},
	{"g", SDL_SCANCODE_G},
	{"h", SDL_SCANCODE_H},
	{"i", SDL_SCANCODE_I},
	{"j", SDL_SCANCODE_J},
	{"k", SDL_SCANCODE_K},
	{"l", SDL_SCANCODE_L},
	{"m", SDL_SCANCODE_M},
	{"n", SDL_SCANCODE_N},
	{"o", SDL_SCANCODE_O},
	{"p", SDL_SCANCODE_P},
	{"q", SDL_SCANCODE_Q},
	{"r", SDL_SCANCODE_R},
	{"s", SDL_SCANCODE_S},
	{"t", SDL_SCANCODE_T},
	{"u", SDL_SCANCODE_U},
	{"v", SDL_SCANCODE_V},
	{"w", SDL_SCANCODE_W},
	{"x", SDL_SCANCODE_X},
	{"y", SDL_SCANCODE_Y},
	{"z", SDL_SCANCODE_Z},

	{"left", SDL_SCANCODE_LEFT},
	{"right", SDL_SCANCODE_RIGHT},
	{"up", SDL_SCANCODE_UP},
	{"down", SDL_SCANCODE_DOWN},

	{"f1", SDL_SCANCODE_F1},
	{"f2", SDL_SCANCODE_F2},
	{"f3", SDL_SCANCODE_F3},
	{"f4", SDL_SCANCODE_F4},
	{"f5", SDL_SCANCODE_F5},
	{"f6", SDL_SCANCODE_F6},
	{"f7", SDL_SCANCODE_F7},
	{"f8", SDL_SCANCODE_F8},
	{"f9", SDL_SCANCODE_F9},
	{"f10", SDL_SCANCODE_F10},
	{"f11", SDL_SCANCODE_F11},
	{"f12", SDL_SCANCODE_F12},
	{"f13", SDL_SCANCODE_F13},
	{"f14", SDL_SCANCODE_F14},
	{"f15", SDL_SCANCODE_F15},
	{"f16", SDL_SCANCODE_F16},
	{"f17", SDL_SCANCODE_F17},
	{"f18", SDL_SCANCODE_F18},
	{"f19", SDL_SCANCODE_F19},
	{"f20", SDL_SCANCODE_F20},
	{"f21", SDL_SCANCODE_F21},
	{"f22", SDL_SCANCODE_F22},
	{"f23", SDL_SCANCODE_F23},
	{"f24", SDL_SCANCODE_F24},

	{"'", SDL_SCANCODE_APOSTROPHE},
	{"\\", SDL_SCANCODE_BACKSLASH},
	{",", SDL_SCANCODE_COMMA},
	{"=", SDL_SCANCODE_EQUALS},
	{"`", SDL_SCANCODE_GRAVE},
	{"[", SDL_SCANCODE_LEFTBRACKET},
	{"-", SDL_SCANCODE_MINUS},
	{".", SDL_SCANCODE_PERIOD},
	{"]", SDL_SCANCODE_RIGHTBRACKET},
	{";", SDL_SCANCODE_SEMICOLON},
	{"/", SDL_SCANCODE_SLASH},
	{" ", SDL_SCANCODE_SPACE},

	{"back", SDL_SCANCODE_BACKSPACE},
	{"delete", SDL_SCANCODE_DELETE},
	{"end", SDL_SCANCODE_END},
	{"home", SDL_SCANCODE_HOME},
	{"insert", SDL_SCANCODE_INSERT},

	{"pgdown", SDL_SCANCODE_PAGEDOWN},
	{"pgup", SDL_SCANCODE_PAGEUP},

	{"return", SDL_SCANCODE_RETURN},
	{"tab", SDL_SCANCODE_TAB},

	{"prntscrn", SDL_SCANCODE_PRINTSCREEN},

	{"caps", SDL_SCANCODE_CAPSLOCK},
	{"esc", SDL_SCANCODE_ESCAPE},
	{"num", SDL_SCANCODE_NUMLOCKCLEAR},
	{"scroll", SDL_SCANCODE_SCROLLLOCK},

	{"lalt", SDL_SCANCODE_LALT},
	{"lctrl", SDL_SCANCODE_LCTRL},
	{"lshift", SDL_SCANCODE_LSHIFT},
	{"ralt", SDL_SCANCODE_RALT},
	{"rctrl", SDL_SCANCODE_RCTRL},
	{"rshift", SDL_SCANCODE_RSHIFT},

	{"menu", SDL_SCANCODE_MENU}
};

const std::unordered_map<SDL_Keycode, std::string> Script::sdl_keys = {
	{SDLK_0, "0"},
	{SDLK_1, "1"},
	{SDLK_2, "2"},
	{SDLK_3, "3"},
	{SDLK_4, "4"},
	{SDLK_5, "5"},
	{SDLK_6, "6"},
	{SDLK_7, "7"},
	{SDLK_8, "8"},
	{SDLK_9, "9"},

	{SDLK_a, "a"},
	{SDLK_b, "b"},
	{SDLK_c, "c"},
	{SDLK_d, "d"},
	{SDLK_e, "e"},
	{SDLK_f, "f"},
	{SDLK_g, "g"},
	{SDLK_h, "h"},
	{SDLK_i, "i"},
	{SDLK_j, "j"},
	{SDLK_k, "k"},
	{SDLK_l, "l"},
	{SDLK_m, "m"},
	{SDLK_n, "n"},
	{SDLK_o, "o"},
	{SDLK_p, "p"},
	{SDLK_q, "q"},
	{SDLK_r, "r"},
	{SDLK_s, "s"},
	{SDLK_t, "t"},
	{SDLK_u, "u"},
	{SDLK_v, "v"},
	{SDLK_w, "w"},
	{SDLK_x, "x"},
	{SDLK_y, "y"},
	{SDLK_z, "z"},

	{SDLK_LEFT, "left"},
	{SDLK_RIGHT, "right"},
	{SDLK_UP, "up"},
	{SDLK_DOWN, "down"},

	{SDLK_F1, "f1"},
	{SDLK_F2, "f2"},
	{SDLK_F3, "f3"},
	{SDLK_F4, "f4"},
	{SDLK_F5, "f5"},
	{SDLK_F6, "f6"},
	{SDLK_F7, "f7"},
	{SDLK_F8, "f8"},
	{SDLK_F9, "f9"},
	{SDLK_F10, "f10"},
	{SDLK_F11, "f11"},
	{SDLK_F12, "f12"},
	{SDLK_F13, "f13"},
	{SDLK_F14, "f14"},
	{SDLK_F15, "f15"},
	{SDLK_F16, "f16"},
	{SDLK_F17, "f17"},
	{SDLK_F18, "f18"},
	{SDLK_F19, "f19"},
	{SDLK_F20, "f20"},
	{SDLK_F21, "f21"},
	{SDLK_F22, "f22"},
	{SDLK_F23, "f23"},
	{SDLK_F24, "f24"},

	{SDLK_QUOTE, "'"},
	{SDLK_BACKSLASH, "\\"},
	{SDLK_COMMA, ","},
	{SDLK_EQUALS, "="},
	{SDLK_BACKQUOTE, "`"},
	{SDLK_LEFTBRACKET, "["},
	{SDLK_MINUS, "-"},
	{SDLK_PERIOD, "."},
	{SDLK_RIGHTBRACKET, "]"},
	{SDLK_SEMICOLON, ";"},
	{SDLK_SLASH, "/"},
	{SDLK_SPACE, " "},

	{SDLK_BACKSPACE, "back"},
	{SDLK_DELETE, "delete"},
	{SDLK_END, "end"},
	{SDLK_HOME, "home"},
	{SDLK_INSERT, "insert"},

	{SDLK_PAGEDOWN, "pgdown"},
	{SDLK_PAGEUP, "pgup"},

	{SDLK_RETURN, "return"},
	{SDLK_TAB, "tab"},

	{SDLK_PRINTSCREEN, "prntscrn"},

	{SDLK_CAPSLOCK, "caps"},
	{SDLK_ESCAPE, "esc"},
	{SDLK_NUMLOCKCLEAR, "num"},
	{SDLK_SCROLLLOCK, "scroll"},

	{SDLK_LALT, "lalt"},
	{SDLK_LCTRL, "lctrl"},
	{SDLK_LSHIFT, "lshift"},
	{SDLK_RALT, "ralt"},
	{SDLK_RCTRL, "rctrl"},
	{SDLK_RSHIFT, "rshift"},

	{SDLK_MENU, "menu"},

	{SDLK_AMPERSAND, "&"},
	{SDLK_ASTERISK, "*"},
	{SDLK_AT, "@"},
	{SDLK_CARET, "^"},
	{SDLK_COLON, ":"},
	{SDLK_DOLLAR, "$"},
	{SDLK_EXCLAIM, "!"},
	{SDLK_GREATER, ">"},
	{SDLK_HASH, "#"},
	{SDLK_LEFTPAREN, "("},
	{SDLK_LESS, "<"},
	{SDLK_PERCENT, "%"},
	{SDLK_PLUS, "+"},
	{SDLK_QUESTION, "?"},
	{SDLK_QUOTEDBL, "\""},
	{SDLK_RIGHTPAREN, ")"},
	{SDLK_UNDERSCORE, "_"}
};

const std::unordered_map<std::string, SDL_Keycode> Script::sdl_rev_keys = {
	{"0", SDLK_0},
	{"1", SDLK_1},
	{"2", SDLK_2},
	{"3", SDLK_3},
	{"4", SDLK_4},
	{"5", SDLK_5},
	{"6", SDLK_6},
	{"7", SDLK_7},
	{"8", SDLK_8},
	{"9", SDLK_9},

	{"a", SDLK_a},
	{"b", SDLK_b},
	{"c", SDLK_c},
	{"d", SDLK_d},
	{"e", SDLK_e},
	{"f", SDLK_f},
	{"g", SDLK_g},
	{"h", SDLK_h},
	{"i", SDLK_i},
	{"j", SDLK_j},
	{"k", SDLK_k},
	{"l", SDLK_l},
	{"m", SDLK_m},
	{"n", SDLK_n},
	{"o", SDLK_o},
	{"p", SDLK_p},
	{"q", SDLK_q},
	{"r", SDLK_r},
	{"s", SDLK_s},
	{"t", SDLK_t},
	{"u", SDLK_u},
	{"v", SDLK_v},
	{"w", SDLK_w},
	{"x", SDLK_x},
	{"y", SDLK_y},
	{"z", SDLK_z},

	{"left", SDLK_LEFT},
	{"right", SDLK_RIGHT},
	{"up", SDLK_UP},
	{"down", SDLK_DOWN},

	{"f1", SDLK_F1},
	{"f2", SDLK_F2},
	{"f3", SDLK_F3},
	{"f4", SDLK_F4},
	{"f5", SDLK_F5},
	{"f6", SDLK_F6},
	{"f7", SDLK_F7},
	{"f8", SDLK_F8},
	{"f9", SDLK_F9},
	{"f10", SDLK_F10},
	{"f11", SDLK_F11},
	{"f12", SDLK_F12},
	{"f13", SDLK_F13},
	{"f14", SDLK_F14},
	{"f15", SDLK_F15},
	{"f16", SDLK_F16},
	{"f17", SDLK_F17},
	{"f18", SDLK_F18},
	{"f19", SDLK_F19},
	{"f20", SDLK_F20},
	{"f21", SDLK_F21},
	{"f22", SDLK_F22},
	{"f23", SDLK_F23},
	{"f24", SDLK_F24},

	{"'", SDLK_QUOTE},
	{"\\", SDLK_BACKSLASH},
	{",", SDLK_COMMA},
	{"=", SDLK_EQUALS},
	{"`", SDLK_BACKQUOTE},
	{"[", SDLK_LEFTBRACKET},
	{"-", SDLK_MINUS},
	{".", SDLK_PERIOD},
	{"]", SDLK_RIGHTBRACKET},
	{";", SDLK_SEMICOLON},
	{"/", SDLK_SLASH},
	{" ", SDLK_SPACE},

	{"back", SDLK_BACKSPACE},
	{"delete", SDLK_DELETE},
	{"end", SDLK_END},
	{"home", SDLK_HOME},
	{"insert", SDLK_INSERT},

	{"pgdown", SDLK_PAGEDOWN},
	{"pgup", SDLK_PAGEUP},

	{"return", SDLK_RETURN},
	{"tab", SDLK_TAB},

	{"prntscrn", SDLK_PRINTSCREEN},

	{"caps", SDLK_CAPSLOCK},
	{"esc", SDLK_ESCAPE},
	{"num", SDLK_NUMLOCKCLEAR},
	{"scroll", SDLK_SCROLLLOCK},

	{"lalt", SDLK_LALT},
	{"lctrl", SDLK_LCTRL},
	{"lshift", SDLK_LSHIFT},
	{"ralt", SDLK_RALT},
	{"rctrl", SDLK_RCTRL},
	{"rshift", SDLK_RSHIFT},

	{"menu", SDLK_MENU},

	{"&", SDLK_AMPERSAND},
	{"*", SDLK_ASTERISK},
	{"@", SDLK_AT},
	{"^", SDLK_CARET},
	{":", SDLK_COLON},
	{"$", SDLK_DOLLAR},
	{"!", SDLK_EXCLAIM},
	{">", SDLK_GREATER},
	{"#", SDLK_HASH},
	{"(", SDLK_LEFTPAREN},
	{"<", SDLK_LESS},
	{"%", SDLK_PERCENT},
	{"+", SDLK_PLUS},
	{"?", SDLK_QUESTION},
	{"\"", SDLK_QUOTEDBL},
	{")", SDLK_RIGHTPAREN},
	{"_", SDLK_UNDERSCORE}
};

const std::unordered_map<Uint8, std::string> Script::sdl_btns = {
	{SDL_BUTTON_LEFT, "left"},
	{SDL_BUTTON_MIDDLE, "middle"},
	{SDL_BUTTON_RIGHT, "right"},
};

const std::unordered_map<std::string, Uint8> Script::sdl_rev_btns = {
	{"left", SDL_BUTTON_LEFT},
	{"middle", SDL_BUTTON_MIDDLE},
	{"right", SDL_BUTTON_RIGHT},
};

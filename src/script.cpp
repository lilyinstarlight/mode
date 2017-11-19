#include <fstream>

#include "background.h"
#include "clock.h"
#include "collision.h"
#include "engine.h"
#include "hud.h"
#include "player.h"
#include "spec.h"
#include "sprite.h"
#include "vector2f.h"
#include "viewport.h"
#include "world.h"

#include "script.h"

Script::Script(const std::string & name, Sprite & s) : path("behaviours"), script(""), lua(), sprite(s) {
	// load file
	load_file(path + "/" + name + ".lua");

	// prepare environment
	load_api();

	// run script
	lua.script(script);
}

Script::Script(const std::string & command) : path("behaviours"), script(command), lua(), sprite(Engine::get_instance().get_world().get_player()) {
	// prepare environment
	load_api();

	// run command
	lua.script(command);
}

Script::Script(const Script & s) : path(s.path), script(s.script), lua(), sprite(s.sprite) {
	// prepare environment
	load_api();

	// run file
	lua.script(script);
}

void Script::load_api() {
	lua.open_libraries(sol::lib::base);

	// create Vector data type
	lua.new_usertype<Vector2f>("Vector",
			"new", sol::constructors<Vector2f(float, float)>(),

			"x", sol::property(&Vector2f::get_x, &Vector2f::set_x),
			"y", sol::property(&Vector2f::get_y, &Vector2f::set_y)
	);

	// create Sprite data type
	lua.new_usertype<Sprite>("Sprite",
			"new", sol::constructors<Sprite(std::string)>(),

			"observe", WrapObserve<Sprite, Sprite>(sprite),
			"ignore", WrapIgnore<Sprite, Sprite>(sprite),

			"inject", &Sprite::inject,

			"state", sol::property(&Sprite::get_state, &Sprite::set_state),

			"width", sol::property(&Sprite::get_width),
			"height", sol::property(&Sprite::get_height),

			"pos", sol::property(&Sprite::get_position, &Sprite::set_position),
			"rot", sol::property(&Sprite::get_rotation, &Sprite::set_rotation),
			"vel", sol::property(&Sprite::get_velocity, &Sprite::set_velocity),
			"scale", sol::property(&Sprite::get_scale, &Sprite::set_scale),
			"idx", sol::property(&Sprite::get_index, &Sprite::set_index)
	);

	// set sprite as current sprite
	lua["sprite"] = &sprite;

	// create Player data type
	lua.new_usertype<Player>("Player",
			"new", sol::no_constructor,

			"observe", WrapObserve<Player, Sprite>(sprite),
			"ignore", WrapIgnore<Player, Sprite>(sprite)
	);

	// set player as current player (nil for player behaviour)
	lua["player"] = &Engine::get_instance().get_world().get_player();

	// create Background data type
	lua.new_usertype<Background>("Background",
			"new", sol::constructors<Background(std::string)>(),

			"factor", sol::property(&Background::get_factor, &Background::set_factor),

			"width", sol::property(&Background::get_width),
			"height", sol::property(&Background::get_height),

			"pos", sol::property(&Background::get_position, &Background::set_position),
			"rot", sol::property(&Background::get_rotation, &Background::set_rotation),
			"vel", sol::property(&Background::get_velocity, &Background::set_velocity),
			"scale", sol::property(&Background::get_scale, &Background::set_scale),
			"idx", sol::property(&Background::get_index, &Background::set_index)
	);

	// create World data type
	lua.new_usertype<World>("World",
			"new", sol::no_constructor,

			"add_sprite", WrapAdd<World, Sprite>(),
			"remove_sprite", WrapRemove<World, Sprite>(),

			"add_background", WrapAdd<World, Background>(),
			"remove_background", WrapRemove<World, Background>(),

			"width", sol::property(&World::get_width),
			"height", sol::property(&World::get_height)
	);

	// set world as current world
	lua["world"] = &Engine::get_instance().get_world();

	// create Input data type
	lua.new_usertype<Input>("Input",
			"new", sol::no_constructor,

			"grab", &Input::grab,
			"release", &Input::release,
			"check", &Input::check,

			"event", sol::property(WrapEvent<Input>(lua)),

			"get_key", WrapGetKey<Input>(lua)
	);

	// set world as current world
	lua["input"] = &Input::get_instance();

	// create Input data type
	lua.new_usertype<Spec>("Spec",
			"new", sol::no_constructor,

			"check", &Spec::check,

			"get_tops", &Spec::get_tops,
			"get_subs", &Spec::get_subs,

			"get_bool", &Spec::get_bool,
			"get_int", &Spec::get_int,
			"get_float", &Spec::get_float,
			"get_str", &Spec::get_str
	);

	// set world as current world
	lua["spec"] = &Spec::get_instance();
}

void Script::load_file(const std::string & filename) {
	// load file contents
	std::ifstream file(filename);
	if (!file)
		throw std::runtime_error("Failed to load script " + filename);

	script = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

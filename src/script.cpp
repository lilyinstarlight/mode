#include <fstream>

#include "background.h"
#include "clock.h"
#include "collision.h"
#include "engine.h"
#include "hud.h"
#include "player.h"
#include "projectile.h"
#include "spec.h"
#include "sprite.h"
#include "vector2f.h"
#include "viewport.h"
#include "world.h"

#include "script.h"

Script::Script() : path("behaviours"), script(""), result(""), lua(), sprite(Engine::get_instance().get_world().get_player()) {}

Script::Script(const std::string & name, Sprite & s) : path("behaviours"), script(""), result(""), lua(), sprite(s) {
	// load file
	load_file(path + "/" + name + ".lua");
}

Script::Script(const Script & s) : path(s.path), script(s.script), result(s.result), lua(), sprite(s.sprite) {}

void Script::load() {
	// prepare environment
	load_api();

	// run script
	result = lua.script(script);
}

void Script::set_script(const std::string & s) {
	std::string old = script;

	try {
		// refresh state
		lua = sol::state();

		// set new script
		script = s;

		// reload
		load();
	}
	catch (std::runtime_error & err) {
		std::string str = err.what();
		result = "> lua error" + str.substr(str.rfind(":"));
	}
}

void Script::add_script(const std::string & s) {
	script += "\n" + s;
	result = lua.script(s);
}

void Script::load_api() {
	lua.open_libraries(sol::lib::base, sol::lib::math);

	// create Vector data type
	lua.new_usertype<Vector2f>("Vector",
			"new", sol::constructors<Vector2f(float, float)>(),

			"x", sol::property(&Vector2f::get_x, &Vector2f::set_x),
			"y", sol::property(&Vector2f::get_y, &Vector2f::set_y)
	);

	// create Sprite data type
	lua.new_usertype<Sprite>("Sprite",
			"new", sol::factories(WrapCreate<World, Sprite, std::string>(Engine::get_instance().get_world())),

			"observe", WrapObserve<Sprite, Sprite>(sprite),
			"ignore", WrapIgnore<Sprite, Sprite>(sprite),

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
	lua["sprite"] = &sprite;

	// create Projectile data type
	lua.new_usertype<Projectile>("Projectile",
			"new", sol::factories(WrapCreate<World, Projectile, std::string>(Engine::get_instance().get_world())),

			"alive", sol::property(&Projectile::is_alive),
			"kill", &Projectile::kill,
			"origin", sol::property(&Projectile::get_origin)
	);

	// set projectile as current sprite if a projectile
	lua["projectile"] = dynamic_cast<Projectile *>(&sprite);

	// create Player data type
	lua.new_usertype<Player>("Player",
			"new", sol::no_constructor,

			"observe", WrapObserve<Player, Sprite>(sprite),
			"ignore", WrapIgnore<Player, Sprite>(sprite),

			"shoot", &Player::shoot
	);

	// set player as current player
	Player * player = dynamic_cast<Player *>(&sprite);
	if (!player)
		player = &Engine::get_instance().get_world().get_player();

	lua["player"] = player;

	// create Background data type
	lua.new_usertype<Background>("Background",
			"new", sol::factories(WrapCreate<World, Background, std::string>(Engine::get_instance().get_world())),

			"factor", sol::property(&Background::get_factor, &Background::set_factor),

			"name", sol::property(&Sprite::get_name),
			"type", sol::property(&Sprite::get_type),

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
			"get_sprite", WrapGet<World, Sprite>(),
			"destroy_sprite", WrapDestroy<World, Sprite>(),

			"add_background", WrapAdd<World, Background>(),
			"remove_background", WrapRemove<World, Background>(),
			"get_background", WrapGet<World, Background>(),
			"destroy_background", WrapDestroy<World, Background>(),

			"add_dialog", WrapAdd<World, Dialog>(),
			"remove_dialog", WrapRemove<World, Dialog>(),
			"get_dialog", WrapGet<World, Dialog>(),
			"destroy_dialog", WrapDestroy<World, Dialog>(),

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

			"get_key", WrapGetKey<Input>(lua)
	);

	// set input
	lua["input"] = &Input::get_instance();

	// create Spec data type
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

	// set spec
	lua["spec"] = &Spec::get_instance();

	// create Engine data type
	lua.new_usertype<Engine>("Engine",
			"new", sol::no_constructor,

			"load", &Engine::load,

			"restart", &Engine::restart,
			"stop", &Engine::stop
	);

	// set engine
	lua["engine"] = &Engine::get_instance();

	// create Clock data type
	lua.new_usertype<Clock>("Clock",
			"new", sol::no_constructor,

			"ticks", sol::property(&Clock::get_ticks),
			"fps", sol::property(&Clock::get_fps)
	);

	// set clock
	lua["clock"] = &Clock::get_instance();

	// create Spec data type
	lua.new_usertype<Dialog>("Dialog",
			"new", sol::factories(WrapCreate<World, Dialog, std::string, std::string>(Engine::get_instance().get_world()), WrapCreate<World, Dialog, std::string, std::string, bool>(Engine::get_instance().get_world()), WrapCreate<World, Dialog, std::string, std::string, bool, bool>(Engine::get_instance().get_world())),

			"name", sol::property(&Sprite::get_name),
			"type", sol::property(&Sprite::get_type),

			"open", &Dialog::open,
			"close", &Dialog::close,
			"toggle", &Dialog::toggle,

			"opened", sol::property(&Dialog::is_open),

			"string", sol::property(&Dialog::get_string, &Dialog::set_string)
	);

	// create Spec data type
	lua.new_usertype<HUD>("HUD",
			"new", sol::no_constructor,

			"open", &HUD::open,
			"close", &HUD::close,
			"toggle", &HUD::toggle,

			"opened", sol::property(&HUD::is_open),

			"string", sol::property(&HUD::get_string, &HUD::set_string)
	);

	// set hud
	lua["hud"] = &Engine::get_instance().get_hud();

	// create Spec data type
	lua.new_usertype<Viewport>("Viewport",
			"new", sol::no_constructor,

			"width", sol::property(&Viewport::get_width),
			"height", sol::property(&Viewport::get_height),

			"pos", sol::property(&Viewport::get_position),

			"track", &Viewport::track
	);

	// set viewport
	lua["view"] = &Engine::get_instance().get_viewport();

	// create Event data type
	lua.new_usertype<SDL_Event>("Event",
			"new", sol::no_constructor,

			"ev", sol::property(WrapEventType(lua)),
			"val", sol::property(WrapEventValue(lua))
	);
}

void Script::load_file(const std::string & filename) {
	// load file contents
	std::ifstream file(filename);
	if (!file)
		throw std::runtime_error("Failed to load script " + filename);

	script = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

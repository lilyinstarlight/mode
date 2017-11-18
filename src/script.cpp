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

// TODO: load other api stuff

Script::Script(const std::string & name, Sprite & s) : path("behaviours"), script(""), lua(), sprite(&s) {
	// load file
	load_file(path + "/" + name + ".lua");

	// prepare environment
	load_api();

	// run script
	lua.script(script);
}

Script::Script(const std::string & command) : path("behaviours"), script(command), lua(), sprite(nullptr) {
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

	// create Sprite data type
	lua.new_userdata<Vector2f>("Vector",
			sol::constructors<sol::types<float, float>>(),

			"x", &Vector2f::x,
			"y", &Vector2f::y
	);

	// create Sprite data type
	lua.new_userdata<Sprite>("Sprite",
			sol::constructors<sol::types<std::string>>(),

			"inject", &Sprite::inject,

			"state", &Sprite::state,

			"pos", &Drawable::position,
			"rot", &Drawable::rotation,
			"vel", &Drawable::velocity,
			"scale", &Drawable::scale,
			"idx", &Drawable::index
	);

	// set sprite as current sprite
	lua["sprite"] = sprite;

	// create World data type
	lua.new_userdata<World>("World",
			"add", &World::add,
			"remove", &World::remove,

			"drawables", &World::drawables,

			"width", &World::width,
			"height", &World::height
	);

	// set world
	lua["world"] = Engine::get_instance().get_world();

	// create World data type
	lua.new_userdata<Player>("Player",
			"hp", &Player::hp,

			"state", &Sprite::state,

			"pos", &Drawable::position,
			"rot", &Drawable::rotation,
			"vel", &Drawable::velocity,
			"scale", &Drawable::scale,
			"idx", &Drawable::index
	);

	// set player
	lua["player"] = Engine::get_instance().get_world().get_player();

	// create World data type
	lua.new_userdata<Background>("Background",
			sol::constructors<sol::types<std::string>>(),

			"width", &Background::width,
			"height", &Background::height,
			"factor", &Background::factor,

			"pos", &Drawable::position,
			"rot", &Drawable::rotation,
			"vel", &Drawable::velocity,
			"scale", &Drawable::scale,
			"idx", &Drawable::index
	);
}

void Script::load_file(const std::string & filename) {
	// load file contents
	std::ifstream file(filename);
	if (!file)
		throw std::runtime_error("Failed to load script " + filename);
	script = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

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
	// load API
	load_api();

	// load file
	load_file(path + "/" + name + ".lua");

	// run file
	lua.script(script);
}

Script::Script(const std::string & command) : path("behaviours"), script(command), lua(), sprite(nullptr) {
	// run command
	lua.script(script);
}

Script::Script(const Script & s) : path(s.path), script(s.script), lua(), sprite(s.sprite) {
	// load API
	load_api();

	// run file
	lua.script(script);
}

void Script::load_api() {
	// load necessary API stuff
	load_sprite();
	load_world();
}

void Script::load_sprite() {
	// create Sprite data type
	lua.new_userdata<Sprite>("Sprite",
			sol::constructors<sol::types<std::string>>(),

			"inject", &Sprite::inject,

			"pos", &Drawable::position,
			"vel", &Drawable::velocity,

			"state", &Sprite::state
	);

	// set sprite as current sprite
	lua["sprite"] = sprite;
}

void Script::load_world() {
	// create World data type
	lua.new_userdata<World>("World",
			"add", &World::add,
			"remove", &World::remove,

			"drawables", &World::drawables,

			"width", &World::width,
			"height", &World::height
	);

	// set sprite as current sprite
	lua["world"] = Engine::get_instance().get_world();
}

void Script::load_player() {
	// create World data type
	lua.new_userdata<Player>("Player",
			"hp", &Player::hp,

			"pos", &Drawable::position,
			"vel", &Drawable::velocity,
			"state", &Sprite::state
	);

	// set sprite as current sprite
	lua["player"] = Engine::get_instance().get_world().get_player();
}

void Script::load_background() {
	// create World data type
	lua.new_userdata<Background>("Background",
			sol::constructors<sol::types<std::string>>(),

			"width", &Background::width,
			"height", &Background::height,
			"factor", &Background::factor,

			"pos", &Drawable::position,
			"vel", &Drawable::velocity
	);
}

void Script::load_file(const std::string & filename) {
	// load file contents
	std::ifstream file(filename);
	if (!file)
		throw std::runtime_error("Failed to load script " + filename);
	script = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

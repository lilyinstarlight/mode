#include <fstream>

#include "clock.h"
#include "collision.h"
#include "hud.h"
#include "player.h"
#include "spec.h"
#include "sprite.h"
#include "vector2f.h"
#include "viewport.h"
#include "world.h"

#include "script.h"

// TODO: load other api stuff

Script::Script(const std::string & name, Sprite & s) : path("behaviours"), script(""), lua(), sprite(&s), player(nullptr) {
	// load necessary API stuff
	load_sprite(*sprite);

	// load file
	load_file(path + "/" + name + ".lua");

	// run file
	lua.script(script);
}

Script::Script(const std::string & name, Player & p) : path("behaviours"), script(""), lua(), sprite(nullptr), player(&p) {
	// load necessary API stuff
	load_sprite(*player);
	load_player(*player);

	// load file
	load_file(path + "/" + name + ".lua");

	// run file
	lua.script(script);
}

Script::Script(const Script & s) : path(s.path), script(s.script), lua(), sprite(s.sprite), player(s.player) {
	// load necessary API stuff
	if (sprite) {
		load_sprite(*sprite);
	}
	else {
		load_sprite(*player);
		load_player(*player);
	}

	// run file
	lua.script(script);
}

void Script::load_sprite(Sprite & sprite) {
	// create Sprite data type
	lua.new_userdata<Sprite>("Sprite",
			sol::constructors<sol::types<std::string, World>>(),
			"pos", &Drawable::position,
			"vel", &Drawable::velocity,
			"state", &Sprite::state
	);

	// set sprite as current sprite
	lua["sprite"] = sprite;
}

void Script::load_player(Player & player) {
	// create Player data type
	lua.new_userdata<Player>("Player",
			sol::constructors<sol::types<World>>(),
			"hp", &Player::hp
	);

	// set sprite as current player
	lua["player"] = player;
}

void Script::load_file(const std::string & filename) {
	// load file contents
	std::ifstream file(filename);
	script = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

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

Script::Script(const std::string & name, Sprite & s) : path("behaviours"), script(), lua(), sprite(&s), player(nullptr) {
	load_sprite(*sprite);

	load_file(path + "/" + name + ".lua");

	lua.script(script);
}

Script::Script(const std::string & name, Player & p) : path("behaviours"), script(), lua(), sprite(nullptr), player(&p) {
	load_sprite(*player);
	load_player(*player);

	load_file(path + "/" + name + ".lua");

	lua.script(script);
}

Script::Script(const Script & s) : path(s.path), script(s.script), lua(), sprite(s.sprite), player(s.player) {
	if (sprite) {
		load_sprite(*sprite);
	}
	else {
		load_sprite(*player);
		load_player(*player);
	}

	lua.script(script);
}

void Script::load_sprite(Sprite & sprite) {
	lua.new_userdata<Sprite>("Sprite",
			"pos", &Drawable::position,
			"vel", &Drawable::velocity
	);

	lua["sprite"] = sprite;
}

void Script::load_player(Player & player) {
	lua.new_userdata<Player>("Player",
			"inject", &Player::inject,
			"hp", &Player::hp
	);

	lua["player"] = player;
}

void Script::load_file(const std::string & filename) {
	std::ifstream file(filename);
	script = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

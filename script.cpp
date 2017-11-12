#include <ifstream>

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

Script::Script(const std::string & name, Sprite & s) : script(), lua(), sprite(&s), player(nullptr) {
	load_sprite(*sprite);

	load_file(path + "/" + name + ".lua");

	lua.script(script);
}

Script::Script(const std::string & name, Player & p) : script(), lua(), sprite(nullptr), player(&p) {
	load_sprite(player->get_sprite());
	load_player(*player);

	load_file(path + "/" + name + ".lua");

	lua.script(script);
}

Script::Script(const Script & s) : script(s.script), lua(), sprite(s.sprite), player(s.player) {
	if (sprite) {
		load_sprite(*sprite);
	}
	else {
		load_sprite(player->get_sprite());
		load_player(*player);
	}

	lua.script(script);
}

Script::load_sprite(Sprite & sprite) {
	lua.new_usertype<Sprite>("Sprite",
			sol::constructors<Sprite(), Sprite(std::string, float, float), Sprite(std::string, float, float, float, float)>(),
			"x", sol::property(&player::get_x, &player::set_x),
			"y", sol::property(&player::get_y, &player::set_y),
			"vel_x", sol::property(&player::get_vel_x, &player::set_vel_x),
			"vel_y", sol::property(&player::get_vel_y, &player::set_vel_y),
	);

	lua["sprite"] = sprite;
}

Script::load_player(Player & player) {
	lua.new_usertype<Player>("Player",
			"inject", &player::inject,
			"hp", sol::property(&player::get_hp, &player::set_hp),
	);

	lua["player"] = sprite;
}

Script::load_file(const std::string & filename) {
	std::ifstream file(filename);
	script = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

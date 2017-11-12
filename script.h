#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>

#include <sol.hpp>

#include "sprite.h"
#include "player.h"

class Script {
	public:
		Script(const std::string & name, Sprite & s);
		Script(const std::string & name, Player & p);

		Script(const Script & s);

		Script() = delete;

		const std::string & get_script() const { return script;                  }
		void set_script(const std::string & s) { script = s; lua.script(script); }

		template<typename... T>
		void call(const std::string & method, T... args) {
			lua[method](args);
		}
	private:
		load_sprite(Sprite & sprite);
		load_player(Sprite & sprite);
		load_file(const std::string & filename);

		std::string script;

		sol::state lua;

		const Sprite * sprite;
		const Player * player;
};
#endif

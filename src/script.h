#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>

#include <sol/sol.hpp>

class Sprite;
class Player;

class Script {
	public:
		Script(const std::string & name, Sprite & s);
		Script(const std::string & name, Player & p);
		Script(const Script & s);

		Script() = delete;
		const Script & operator=(const Script & s) = delete;

		const std::string & get_script() const { return script;                  }
		void set_script(const std::string & s) { script = s; lua.script(script); }

		template<typename... T>
		void call(const std::string & method, T... args) {
			lua[method].call<T...>(args...);
		}
	private:
		void load_sprite(Sprite & sprite);
		void load_player(Player & player);
		void load_file(const std::string & filename);

		std::string path;

		std::string script;

		sol::state lua;

		Sprite * sprite;
		Player * player;
};
#endif

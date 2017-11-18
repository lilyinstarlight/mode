#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>
#include <vector>

#include <sol2/sol.hpp>

class Sprite;

class Script {
	public:
		Script(const std::string & name, Sprite & s);
		Script(const std::string & command);
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
		void load_api();
		void load_file(const std::string & filename);

		std::string path;

		std::string script;
		sol::state lua;
		Sprite * sprite;
};
#endif

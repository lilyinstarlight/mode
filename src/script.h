#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>
#include <vector>

#include <sol/sol.hpp>

class Sprite;

class Script {
	public:
		class API {
			public:
				virtual ~API() {}

				virtual void load() = 0;
				virtual void write() = 0;
		};

		Script(const std::string & name, Sprite & s);
		Script(const std::string & command);
		Script(const Script & s);
		~Script();

		Script() = delete;
		const Script & operator=(const Script & s) = delete;

		const std::string & get_script() const { return script;                  }
		void set_script(const std::string & s) { script = s; lua.script(script); }

		template<typename... T>
		void call(const std::string & method, T... args) {
			load();
			lua[method].call<T...>(args...);
			write();
		}
	private:
		void load_api();
		void load_file(const std::string & filename);

		void load();
		void write();

		std::string path;

		std::string script;
		sol::state lua;
		std::vector<API *> apis;
		Sprite * sprite;
};
#endif

#ifndef SCRIPT_H
#define SCRIPT_H
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <SDL.h>

#include "sol3.hpp"

class Sprite;

class Script {
	public:
		Script();
		Script(const std::string & name, Sprite & sprite);
		Script(const Script & s);

		~Script() {}

		const Script & operator=(const Script &) = delete;

		void load();

		const std::string & get_script() const { return _script; }
		void set_script(const std::string & script);

		std::tuple<int, std::string> repl(const std::string & script);

		template <typename... Args>
		void call(const std::string & method, Args... args) {
			_lua[method](args...);
		}

		template <typename T, typename... Args>
		T call(const std::string & method, Args... args) {
			return _lua[method](args...);
		}

		template <typename... Args>
		void signal(const std::string & method, Args... args) {
			sol::optional<sol::function> fun = _lua[method];
			if (fun)
				(*fun)(args...);
		}

		template <typename T, typename... Args>
		T call_optional(const std::string & method, const T & fallback, Args... args) {
			sol::optional<sol::function> fun = _lua[method];
			if (fun)
				return (*fun)(args...);
			else
				return fallback;
		}

	private:
		static const std::unordered_map<SDL_Scancode, std::string> sdl_codes;
		static const std::unordered_map<std::string, SDL_Scancode> sdl_rev_codes;
		static const std::unordered_map<SDL_Keycode, std::string> sdl_keys;
		static const std::unordered_map<std::string, SDL_Keycode> sdl_rev_keys;
		static const std::unordered_map<Uint8, std::string> sdl_btns;
		static const std::unordered_map<std::string, Uint8> sdl_rev_btns;

		template <typename T, typename U>
		class WrapObserve {
			public:
				WrapObserve(U & other) : _other(other) {}

				void operator()(T & obj) {
					obj.observe(_other);
				}

			private:
				U & _other;
		};

		template <typename T, typename U>
		class WrapIgnore {
			public:
				WrapIgnore(U & other) : _other(other) {}

				void operator()(T & obj) {
					obj.ignore(_other);
				}

			private:
				U & _other;
		};

		template <typename T, typename U, typename... Args>
		class WrapCreate {
			public:
				WrapCreate(T & obj) : _obj(obj) {}

				U * operator()(Args... args) {
					return _obj.template create<U>(args...);
				}

			private:
				T & _obj;
		};

		template <typename T, typename U>
		class WrapAdd {
			public:
				void operator()(T & obj, U & other) {
					obj.add(other);
				}
		};

		template <typename T, typename U>
		class WrapRemove {
			public:
				void operator()(T & obj, U & other) {
					obj.remove(other);
				}
		};

		template <typename T, typename U>
		class WrapCheck {
			public:
				bool operator()(T & obj, U * other) {
					return obj.check(other);
				}
		};

		template <typename T, typename U>
		class WrapGet {
			public:
				U * operator()(T & obj, const std::string & name) {
					return dynamic_cast<U *>(obj.get(name));
				}
		};

		template <typename T, typename U>
		class WrapDestroy {
			public:
				void operator()(T & obj, U * u) {
					return obj.destroy(u);
				}
		};

		template <typename T>
		class WrapGetScancodeList {
			public:
				WrapGetScancodeList(std::vector<SDL_Scancode> (T::* method)() const) : _method(method) {}

				std::vector<std::string> operator()(T & input) {
					std::vector<std::string> codes;
					for (const SDL_Scancode & code : (input.*_method)())
						codes.push_back(sdl_codes.at(code));
					return codes;
				}

			private:
				std::vector<SDL_Scancode> (T::* _method)() const;
		};

		template <typename T>
		class WrapGetScancode {
			public:
				WrapGetScancode(bool (T::* method)(const SDL_Scancode &) const) : _method(method) {}

				bool operator()(T & input, std::string key) {
					return (input.*_method)(sdl_rev_codes.at(key));
				}

			private:
				bool (T::* _method)(const SDL_Scancode &) const;
		};

		class WrapEventType {
			public:
				WrapEventType(sol::state &) {}

				std::string operator()(const SDL_Event & event) {
					switch (event.type) {
						case SDL_KEYDOWN:
							return "keydown";

						case SDL_KEYUP:
							return "keyup";

						case SDL_MOUSEMOTION:
							return "motion";

						case SDL_MOUSEBUTTONDOWN:
							return "btndown";

						case SDL_MOUSEBUTTONUP:
							return "btnup";

						default:
							return "other";
					}
				}
		};

		class WrapEventValue {
			public:
				WrapEventValue(sol::state & lua) : _lua(lua) {}

				sol::table operator()(const SDL_Event & event) {
					switch (event.type) {
						case SDL_KEYDOWN:
						case SDL_KEYUP:
							return _lua.create_table_with(
								"code", sdl_codes.at(event.key.keysym.scancode),
								"key", sdl_keys.at(event.key.keysym.sym),
								"rep", event.key.repeat
							);

						case SDL_MOUSEMOTION:
							return _lua.create_table_with(
								"rx", event.motion.xrel,
								"ry", event.motion.yrel,
								"mx", event.motion.x,
								"my", event.motion.y
							);

						case SDL_MOUSEBUTTONDOWN:
						case SDL_MOUSEBUTTONUP:
							return _lua.create_table_with(
								"btn", sdl_btns.at(event.button.button),
								"mx", event.button.x,
								"my", event.button.y
							);

						default:
							return _lua.create_table_with();
					}
				}

			private:
				sol::state & _lua;
		};

		void load_api();
		void load_file(const std::string & filename);

		std::string _path;

		bool _interactive;
		std::string _script;

		sol::state _lua;

		Sprite & _sprite;
};
#endif

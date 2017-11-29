#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL.h>

#include <sol2/sol.hpp>

class Sprite;

class Script {
	public:
		Script();
		Script(const std::string & name, Sprite & s);
		Script(const Script & s);

		const Script & operator=(const Script & s) = delete;

		const std::string & get_script() const { return script;                              }
		void add_script(const std::string & s) { script += "\n" + s; result = lua.script(s); }

		const std::string & get_result() const { return result; }

		template<typename... T>
		void call(const std::string & method, T... args) {
			lua[method].call<T...>(args...);
		}

	private:

		template <typename T, typename U>
		class WrapObserve {
			public:
				WrapObserve(U & o) : other(o) {}

				void operator()(T & obj) {
					obj.observe(other);
				}

			private:
				U & other;
		};

		template <typename T, typename U>
		class WrapIgnore {
			public:
				WrapIgnore(U & o) : other(o) {}

				void operator()(T & obj) {
					obj.ignore(other);
				}

			private:
				U & other;
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
		class WrapGet {
			public:
				U * operator()(T & obj, const std::string & name) {
					return dynamic_cast<U *>(obj.get(name));
				}
		};

		template <typename T>
		class WrapGetKey {
			public:
				WrapGetKey(sol::state & l) : lua(l) {}

				bool operator()(T & input, std::string key) {
					static std::unordered_map<std::string, SDL_Scancode> codes = {
						{"0", SDL_SCANCODE_0},
						{"1", SDL_SCANCODE_1},
						{"2", SDL_SCANCODE_2},
						{"3", SDL_SCANCODE_3},
						{"4", SDL_SCANCODE_4},
						{"5", SDL_SCANCODE_5},
						{"6", SDL_SCANCODE_6},
						{"7", SDL_SCANCODE_7},
						{"8", SDL_SCANCODE_8},
						{"9", SDL_SCANCODE_9},

						{"a", SDL_SCANCODE_A},
						{"b", SDL_SCANCODE_B},
						{"c", SDL_SCANCODE_C},
						{"d", SDL_SCANCODE_D},
						{"e", SDL_SCANCODE_E},
						{"f", SDL_SCANCODE_F},
						{"g", SDL_SCANCODE_G},
						{"h", SDL_SCANCODE_H},
						{"i", SDL_SCANCODE_I},
						{"j", SDL_SCANCODE_J},
						{"k", SDL_SCANCODE_K},
						{"l", SDL_SCANCODE_L},
						{"m", SDL_SCANCODE_M},
						{"n", SDL_SCANCODE_N},
						{"o", SDL_SCANCODE_O},
						{"p", SDL_SCANCODE_P},
						{"q", SDL_SCANCODE_Q},
						{"r", SDL_SCANCODE_R},
						{"s", SDL_SCANCODE_S},
						{"t", SDL_SCANCODE_T},
						{"u", SDL_SCANCODE_U},
						{"v", SDL_SCANCODE_V},
						{"w", SDL_SCANCODE_W},
						{"x", SDL_SCANCODE_X},
						{"y", SDL_SCANCODE_Y},
						{"z", SDL_SCANCODE_Z},

						{"left", SDL_SCANCODE_LEFT},
						{"right", SDL_SCANCODE_RIGHT},
						{"up", SDL_SCANCODE_UP},
						{"down", SDL_SCANCODE_DOWN},

						{"f1", SDL_SCANCODE_F1},
						{"f2", SDL_SCANCODE_F2},
						{"f3", SDL_SCANCODE_F3},
						{"f4", SDL_SCANCODE_F4},
						{"f5", SDL_SCANCODE_F5},
						{"f6", SDL_SCANCODE_F6},
						{"f7", SDL_SCANCODE_F7},
						{"f8", SDL_SCANCODE_F8},
						{"f9", SDL_SCANCODE_F9},
						{"f10", SDL_SCANCODE_F10},
						{"f11", SDL_SCANCODE_F11},
						{"f12", SDL_SCANCODE_F12},
						{"f13", SDL_SCANCODE_F13},
						{"f14", SDL_SCANCODE_F14},
						{"f15", SDL_SCANCODE_F15},
						{"f16", SDL_SCANCODE_F16},
						{"f17", SDL_SCANCODE_F17},
						{"f18", SDL_SCANCODE_F18},
						{"f19", SDL_SCANCODE_F19},
						{"f20", SDL_SCANCODE_F20},
						{"f21", SDL_SCANCODE_F21},
						{"f22", SDL_SCANCODE_F22},
						{"f23", SDL_SCANCODE_F23},
						{"f24", SDL_SCANCODE_F24},

						{"'", SDL_SCANCODE_APOSTROPHE},
						{"\\", SDL_SCANCODE_BACKSLASH},
						{",", SDL_SCANCODE_COMMA},
						{"=", SDL_SCANCODE_EQUALS},
						{"`", SDL_SCANCODE_GRAVE},
						{"[", SDL_SCANCODE_LEFTBRACKET},
						{"-", SDL_SCANCODE_MINUS},
						{".", SDL_SCANCODE_PERIOD},
						{"]", SDL_SCANCODE_RIGHTBRACKET},
						{";", SDL_SCANCODE_SEMICOLON},
						{"/", SDL_SCANCODE_SLASH},
						{" ", SDL_SCANCODE_SPACE},

						{"back", SDL_SCANCODE_BACKSPACE},
						{"delete", SDL_SCANCODE_DELETE},
						{"end", SDL_SCANCODE_END},
						{"home", SDL_SCANCODE_HOME},
						{"insert", SDL_SCANCODE_INSERT},

						{"pgdown", SDL_SCANCODE_PAGEDOWN},
						{"pgup", SDL_SCANCODE_PAGEUP},

						{"return", SDL_SCANCODE_RETURN},
						{"tab", SDL_SCANCODE_TAB},

						{"prntscrn", SDL_SCANCODE_PRINTSCREEN},

						{"caps", SDL_SCANCODE_CAPSLOCK},
						{"esc", SDL_SCANCODE_ESCAPE},
						{"num", SDL_SCANCODE_NUMLOCKCLEAR},
						{"scroll", SDL_SCANCODE_SCROLLLOCK},

						{"lalt", SDL_SCANCODE_LALT},
						{"lctrl", SDL_SCANCODE_LCTRL},
						{"lshift", SDL_SCANCODE_LSHIFT},
						{"ralt", SDL_SCANCODE_RALT},
						{"rctrl", SDL_SCANCODE_RCTRL},
						{"rshift", SDL_SCANCODE_RSHIFT},

						{"menu", SDL_SCANCODE_MENU}
					};

					return input.get_key(codes[key]);
				}

			private:
				sol::state & lua;
		};

		class WrapEventType {
			public:
				WrapEventType(sol::state & l) : lua(l) {}

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

			private:
				sol::state & lua;
		};

		class WrapEventValue {
			public:
				WrapEventValue(sol::state & l) : lua(l) {}

				sol::table operator()(const SDL_Event & event) {
					static std::unordered_map<SDL_Keycode, std::string> keys = {
						{SDLK_0, "0"},
						{SDLK_1, "1"},
						{SDLK_2, "2"},
						{SDLK_3, "3"},
						{SDLK_4, "4"},
						{SDLK_5, "5"},
						{SDLK_6, "6"},
						{SDLK_7, "7"},
						{SDLK_8, "8"},
						{SDLK_9, "9"},

						{SDLK_a, "a"},
						{SDLK_b, "b"},
						{SDLK_c, "c"},
						{SDLK_d, "d"},
						{SDLK_e, "e"},
						{SDLK_f, "f"},
						{SDLK_g, "g"},
						{SDLK_h, "h"},
						{SDLK_i, "i"},
						{SDLK_j, "j"},
						{SDLK_k, "k"},
						{SDLK_l, "l"},
						{SDLK_m, "m"},
						{SDLK_n, "n"},
						{SDLK_o, "o"},
						{SDLK_p, "p"},
						{SDLK_q, "q"},
						{SDLK_r, "r"},
						{SDLK_s, "s"},
						{SDLK_t, "t"},
						{SDLK_u, "u"},
						{SDLK_v, "v"},
						{SDLK_w, "w"},
						{SDLK_x, "x"},
						{SDLK_y, "y"},
						{SDLK_z, "z"},

						{SDLK_LEFT, "left"},
						{SDLK_RIGHT, "right"},
						{SDLK_UP, "up"},
						{SDLK_DOWN, "down"},

						{SDLK_F1, "f1"},
						{SDLK_F2, "f2"},
						{SDLK_F3, "f3"},
						{SDLK_F4, "f4"},
						{SDLK_F5, "f5"},
						{SDLK_F6, "f6"},
						{SDLK_F7, "f7"},
						{SDLK_F8, "f8"},
						{SDLK_F9, "f9"},
						{SDLK_F10, "f10"},
						{SDLK_F11, "f11"},
						{SDLK_F12, "f12"},
						{SDLK_F13, "f13"},
						{SDLK_F14, "f14"},
						{SDLK_F15, "f15"},
						{SDLK_F16, "f16"},
						{SDLK_F17, "f17"},
						{SDLK_F18, "f18"},
						{SDLK_F19, "f19"},
						{SDLK_F20, "f20"},
						{SDLK_F21, "f21"},
						{SDLK_F22, "f22"},
						{SDLK_F23, "f23"},
						{SDLK_F24, "f24"},

						{SDLK_QUOTE, "'"},
						{SDLK_BACKSLASH, "\\"},
						{SDLK_COMMA, ","},
						{SDLK_EQUALS, "="},
						{SDLK_BACKQUOTE, "`"},
						{SDLK_LEFTBRACKET, "["},
						{SDLK_MINUS, "-"},
						{SDLK_PERIOD, "."},
						{SDLK_RIGHTBRACKET, "]"},
						{SDLK_SEMICOLON, ";"},
						{SDLK_SLASH, "/"},
						{SDLK_SPACE, " "},

						{SDLK_BACKSPACE, "back"},
						{SDLK_DELETE, "delete"},
						{SDLK_END, "end"},
						{SDLK_HOME, "home"},
						{SDLK_INSERT, "insert"},

						{SDLK_PAGEDOWN, "pgdown"},
						{SDLK_PAGEUP, "pgup"},

						{SDLK_RETURN, "return"},
						{SDLK_TAB, "tab"},

						{SDLK_PRINTSCREEN, "prntscrn"},

						{SDLK_CAPSLOCK, "caps"},
						{SDLK_ESCAPE, "esc"},
						{SDLK_NUMLOCKCLEAR, "num"},
						{SDLK_SCROLLLOCK, "scroll"},

						{SDLK_LALT, "lalt"},
						{SDLK_LCTRL, "lctrl"},
						{SDLK_LSHIFT, "lshift"},
						{SDLK_RALT, "ralt"},
						{SDLK_RCTRL, "rctrl"},
						{SDLK_RSHIFT, "rshift"},

						{SDLK_MENU, "menu"},

						{SDLK_AMPERSAND, "&"},
						{SDLK_ASTERISK, "*"},
						{SDLK_AT, "@"},
						{SDLK_CARET, "^"},
						{SDLK_COLON, ":"},
						{SDLK_DOLLAR, "$"},
						{SDLK_EXCLAIM, "!"},
						{SDLK_GREATER, ">"},
						{SDLK_HASH, "#"},
						{SDLK_LEFTPAREN, "("},
						{SDLK_LESS, "<"},
						{SDLK_PERCENT, "%"},
						{SDLK_PLUS, "+"},
						{SDLK_QUESTION, "?"},
						{SDLK_QUOTEDBL, "\""},
						{SDLK_RIGHTPAREN, ")"},
						{SDLK_UNDERSCORE, "_"}
					};

					static std::unordered_map<Uint8, std::string> btns = {
						{SDL_BUTTON_LEFT, "left"},
						{SDL_BUTTON_MIDDLE, "middle"},
						{SDL_BUTTON_RIGHT, "right"},
					};

					switch (event.type) {
						case SDL_KEYDOWN:
						case SDL_KEYUP:
							return lua.create_table_with(
								"key", keys[event.key.keysym.sym],
								"rep", event.key.repeat
							);

						case SDL_MOUSEMOTION:
							return lua.create_table_with(
								"rx", event.motion.xrel,
								"ry", event.motion.yrel,
								"mx", event.motion.x,
								"my", event.motion.y
							);

						case SDL_MOUSEBUTTONDOWN:
						case SDL_MOUSEBUTTONUP:
							return lua.create_table_with(
								"btn", btns[event.button.button],
								"mx", event.button.x,
								"my", event.button.y
							);

						default:
							return lua.create_table_with();
					}
				}

			private:
				sol::state & lua;
		};

		void load_api();
		void load_file(const std::string & filename);

		std::string path;

		std::string script;
		std::string result;

		sol::state lua;
		Sprite & sprite;
};
#endif

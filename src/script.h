#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>
#include <unordered_map>
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

		template <typename T>
		class WrapEvent {
			public:
				WrapEvent(sol::state & l) : lua(l) {}

				sol::table * operator()(T & input) {
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

						{SDLK_UP, "up"},
						{SDLK_LEFT, "left"},
						{SDLK_DOWN, "down"},
						{SDLK_RIGHT, "right"},

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

					static sol::table event;

					const SDL_Event & ev = input.get_event();

					switch (ev.type) {
						case SDL_KEYDOWN:
							event["type"] = "keydown";

							event["key"] = keys[ev.key.keysym.sym];
							break;

						case SDL_KEYUP:
							event["type"] = "keyup";

							event["key"] = keys[ev.key.keysym.sym];
							break;

						case SDL_MOUSEMOTION:
							event["type"] = "motion";
							event["rx"] = ev.motion.xrel;
							event["ry"] = ev.motion.yrel;

							event["mx"] = ev.motion.x;
							event["my"] = ev.motion.y;
							break;

						case SDL_MOUSEBUTTONDOWN:
							event["type"] = "btndown";

							event["btn"] = btns[ev.button.button];

							event["mx"] = ev.button.x;
							event["my"] = ev.button.y;
							break;

						case SDL_MOUSEBUTTONUP:
							event["type"] = "btnup";

							event["btn"] = btns[ev.button.button];

							event["mx"] = ev.button.x;
							event["my"] = ev.button.y;
							break;

						default:
							event["type"] = "other";
					}

					return &event;
				}

			private:
				sol::state & lua;
		};

		template <typename T>
		class WrapKeystate {
			public:
				WrapKeystate(sol::state & l) : lua(l) {}

				sol::table * operator()(T & input) {
					static std::unordered_map<SDL_Scancode, std::string> codes = {
						{SDL_SCANCODE_0, "0"},
						{SDL_SCANCODE_1, "1"},
						{SDL_SCANCODE_2, "2"},
						{SDL_SCANCODE_3, "3"},
						{SDL_SCANCODE_4, "4"},
						{SDL_SCANCODE_5, "5"},
						{SDL_SCANCODE_6, "6"},
						{SDL_SCANCODE_7, "7"},
						{SDL_SCANCODE_8, "8"},
						{SDL_SCANCODE_9, "9"},

						{SDL_SCANCODE_A, "a"},
						{SDL_SCANCODE_B, "b"},
						{SDL_SCANCODE_C, "c"},
						{SDL_SCANCODE_D, "d"},
						{SDL_SCANCODE_E, "e"},
						{SDL_SCANCODE_F, "f"},
						{SDL_SCANCODE_G, "g"},
						{SDL_SCANCODE_H, "h"},
						{SDL_SCANCODE_I, "i"},
						{SDL_SCANCODE_J, "j"},
						{SDL_SCANCODE_K, "k"},
						{SDL_SCANCODE_L, "l"},
						{SDL_SCANCODE_M, "m"},
						{SDL_SCANCODE_N, "n"},
						{SDL_SCANCODE_O, "o"},
						{SDL_SCANCODE_P, "p"},
						{SDL_SCANCODE_Q, "q"},
						{SDL_SCANCODE_R, "r"},
						{SDL_SCANCODE_S, "s"},
						{SDL_SCANCODE_T, "t"},
						{SDL_SCANCODE_U, "u"},
						{SDL_SCANCODE_V, "v"},
						{SDL_SCANCODE_W, "w"},
						{SDL_SCANCODE_X, "x"},
						{SDL_SCANCODE_Y, "y"},
						{SDL_SCANCODE_Z, "z"},

						{SDL_SCANCODE_UP, "up"},
						{SDL_SCANCODE_LEFT, "left"},
						{SDL_SCANCODE_DOWN, "down"},
						{SDL_SCANCODE_RIGHT, "right"},

						{SDL_SCANCODE_F1, "f1"},
						{SDL_SCANCODE_F2, "f2"},
						{SDL_SCANCODE_F3, "f3"},
						{SDL_SCANCODE_F4, "f4"},
						{SDL_SCANCODE_F5, "f5"},
						{SDL_SCANCODE_F6, "f6"},
						{SDL_SCANCODE_F7, "f7"},
						{SDL_SCANCODE_F8, "f8"},
						{SDL_SCANCODE_F9, "f9"},
						{SDL_SCANCODE_F10, "f10"},
						{SDL_SCANCODE_F11, "f11"},
						{SDL_SCANCODE_F12, "f12"},
						{SDL_SCANCODE_F13, "f13"},
						{SDL_SCANCODE_F14, "f14"},
						{SDL_SCANCODE_F15, "f15"},
						{SDL_SCANCODE_F16, "f16"},
						{SDL_SCANCODE_F17, "f17"},
						{SDL_SCANCODE_F18, "f18"},
						{SDL_SCANCODE_F19, "f19"},
						{SDL_SCANCODE_F20, "f20"},
						{SDL_SCANCODE_F21, "f21"},
						{SDL_SCANCODE_F22, "f22"},
						{SDL_SCANCODE_F23, "f23"},
						{SDL_SCANCODE_F24, "f24"},

						{SDL_SCANCODE_APOSTROPHE, "'"},
						{SDL_SCANCODE_BACKSLASH, "\\"},
						{SDL_SCANCODE_COMMA, ","},
						{SDL_SCANCODE_EQUALS, "="},
						{SDL_SCANCODE_GRAVE, "`"},
						{SDL_SCANCODE_LEFTBRACKET, "["},
						{SDL_SCANCODE_MINUS, "-"},
						{SDL_SCANCODE_PERIOD, "."},
						{SDL_SCANCODE_RIGHTBRACKET, "]"},
						{SDL_SCANCODE_SEMICOLON, ";"},
						{SDL_SCANCODE_SLASH, "/"},
						{SDL_SCANCODE_SPACE, " "},

						{SDL_SCANCODE_BACKSPACE, "back"},
						{SDL_SCANCODE_DELETE, "delete"},
						{SDL_SCANCODE_END, "end"},
						{SDL_SCANCODE_HOME, "home"},
						{SDL_SCANCODE_INSERT, "insert"},

						{SDL_SCANCODE_PAGEDOWN, "pgdown"},
						{SDL_SCANCODE_PAGEUP, "pgup"},

						{SDL_SCANCODE_RETURN, "return"},
						{SDL_SCANCODE_TAB, "tab"},

						{SDL_SCANCODE_PRINTSCREEN, "prntscrn"},

						{SDL_SCANCODE_CAPSLOCK, "caps"},
						{SDL_SCANCODE_ESCAPE, "esc"},
						{SDL_SCANCODE_NUMLOCKCLEAR, "num"},
						{SDL_SCANCODE_SCROLLLOCK, "scroll"},

						{SDL_SCANCODE_LALT, "lalt"},
						{SDL_SCANCODE_LCTRL, "lctrl"},
						{SDL_SCANCODE_LSHIFT, "lshift"},
						{SDL_SCANCODE_RALT, "ralt"},
						{SDL_SCANCODE_RCTRL, "rctrl"},
						{SDL_SCANCODE_RSHIFT, "rshift"},

						{SDL_SCANCODE_MENU, "menu"}
					};

					static sol::table keystate;

					for (const std::pair<SDL_Scancode, std::string> & code : codes)
						keystate[code.second] = input.get_keystate()[code.first];

					return &keystate;
				}

			private:
				sol::state & lua;
		};

		void load_api();
		void load_file(const std::string & filename);

		std::string path;

		std::string script;
		sol::state lua;
		Sprite & sprite;
};
#endif

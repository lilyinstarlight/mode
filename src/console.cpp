#include <tuple>

#include "context.h"
#include "engine.h"
#include "script.h"
#include "spec.h"
#include "text.h"

#include "console.h"

Console::Console() : Drawable("console",
			"console",
			Vector2f(0, 0),
			0,
			Vector2f(0, 0), 1, 9003),
		_script(nullptr),
		_opened(false),
		_command(""),
		_status(0),
		_result(""),
		_prompt("> _"),
		_font(Spec::get_instance().check("console/font") ? "console/font" : "font/default"),
		_color{static_cast<Uint8>(Spec::get_instance().get_int("console/text/r")), static_cast<Uint8>(Spec::get_instance().get_int("console/text/g")), static_cast<Uint8>(Spec::get_instance().get_int("console/text/b")), static_cast<Uint8>(Spec::get_instance().get_int("console/text/a"))},
		_box{static_cast<Uint8>(Spec::get_instance().get_int("console/box/r")), static_cast<Uint8>(Spec::get_instance().get_int("console/box/g")), static_cast<Uint8>(Spec::get_instance().get_int("console/box/b")), static_cast<Uint8>(Spec::get_instance().get_int("console/box/a"))},
		_text(Text::get_instance().write(_font, _prompt, _color)),
		_size{0, 0, 0, 0},
		_padding_bottom(4), _padding_left(5), _padding_text(2) {}

Console::Console(const Console & console) : Drawable(console), _script(console._script), _opened(console._opened), _command(console._command), _status(console._status), _result(console._result), _prompt(console._prompt), _font(console._font), _color(console._color), _box(console._box), _text(Text::get_instance().write(_font, _prompt, _color)), _size{0, 0, 0, 0}, _padding_bottom(console._padding_bottom), _padding_left(console._padding_left), _padding_text(console._padding_text) {}

Console::~Console() {
	if (_script)
		delete _script;
}

void Console::dispatch(const SDL_Event & event) {
	if (_opened) {
		// grab keyboard focus
		if (!Input::get_instance().check("console")) {
			Input::get_instance().grab("console");
			SDL_StartTextInput();

			if (_script)
				delete _script;

			_script = new Script();
			_script->load();
		}

		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_BACKSPACE) {
				// backspace
				if (_result.empty()) {
					if (!_command.empty())
						_command.pop_back();
				}
				else {
					_result.clear();
				}
			}
			else if (event.key.keysym.sym == SDLK_RETURN) {
				if (_result.empty()) {
					std::tie(_status, _result) = _script->repl(_command);

					_command.clear();
				}
				else {
					_result.clear();
				}
			}
		}
		else if (event.type == SDL_TEXTINPUT && std::string(event.text.text) != "`") {
			if (_result.empty())
				// record text
				_command += event.text.text;
			else
				_result.clear();
		}
	}
	else {
		// release keyboard focus
		if (Input::get_instance().check("console")) {
			Input::get_instance().release("console");
			SDL_StopTextInput();

			_command.clear();
			_result.clear();

			if (_script)
				delete _script;

			_script = nullptr;
		}
	}
}

void Console::update(unsigned int, World &) {
	if (_opened) {
		// get correct prompt
		switch (_status) {
			case 2:
				_prompt = ">> " + _command + "_";
				break;

			case 1:
			case 3:
			case 0:
			default:
				if (_result.empty())
					_prompt = "> " + _command + "_";
				else
					_prompt = _result;
				break;
		}

		// draw text
		Text::get_instance().destroy(_text);
		_text = Text::get_instance().write(_font, _prompt, _color);

		// calculate size
		_size.w = Engine::get_instance().get_viewport().get_width() - 2*_padding_left;
		_size.h = _text->h + 2*_padding_text;

		_size.x = _padding_left;
		_size.y = Engine::get_instance().get_viewport().get_height() - _size.h - _padding_bottom - _padding_text;
	}
}

void Console::draw(const Viewport &) const {
	if (_opened) {
		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), _box.r, _box.g, _box.b, _box.a);
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &_size);

		// draw text
		Text::get_instance().render(Context::get_instance().get_renderer(), _text, _size.x + _padding_text, _size.y + _padding_text, 0);
	}
}

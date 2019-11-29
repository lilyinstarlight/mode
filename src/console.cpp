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
		script(nullptr),
		opened(false),
		command(""),
		status(0),
		result(""),
		prompt("> _"),
		font(Spec::get_instance().check("console/font") ? "console/font" : "font/default"),
		color{static_cast<Uint8>(Spec::get_instance().get_int("console/text/r")), static_cast<Uint8>(Spec::get_instance().get_int("console/text/g")), static_cast<Uint8>(Spec::get_instance().get_int("console/text/b")), static_cast<Uint8>(Spec::get_instance().get_int("console/text/a"))},
		box{static_cast<Uint8>(Spec::get_instance().get_int("console/box/r")), static_cast<Uint8>(Spec::get_instance().get_int("console/box/g")), static_cast<Uint8>(Spec::get_instance().get_int("console/box/b")), static_cast<Uint8>(Spec::get_instance().get_int("console/box/a"))},
		text(Text::get_instance().write(font, prompt, color)),
		size{0, 0, 0, 0},
		padding_bottom(4), padding_left(5), padding_text(2) {}

Console::Console(const Console & console) : Drawable(console), script(console.script), opened(console.opened), command(console.command), status(console.status), result(console.result), prompt(console.prompt), font(console.font), color(console.color), box(console.box), text(Text::get_instance().write(font, prompt, color)), size{0, 0, 0, 0}, padding_bottom(console.padding_bottom), padding_left(console.padding_left), padding_text(console.padding_text) {}

Console::~Console() {
	if (script)
		delete script;
}

void Console::dispatch(const SDL_Event & event) {
	if (opened) {
		// grab keyboard focus
		if (!Input::get_instance().check("console")) {
			Input::get_instance().grab("console");
			SDL_StartTextInput();

			if (script)
				delete script;

			script = new Script();
			script->load();
		}

		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_BACKSPACE) {
				// backspace
				if (result.empty()) {
					if (!command.empty())
						command.pop_back();
				}
				else {
					result.clear();
				}
			}
			else if (event.key.keysym.sym == SDLK_RETURN) {
				if (result.empty()) {
					std::tie(status, result) = script->repl(command);

					command.clear();
				}
				else {
					result.clear();
				}
			}
		}
		else if (event.type == SDL_TEXTINPUT && std::string(event.text.text) != "`") {
			// record text
			command += event.text.text;
		}
	}
	else {
		// release keyboard focus
		if (Input::get_instance().check("console")) {
			Input::get_instance().release("console");
			SDL_StopTextInput();

			command.clear();
			result.clear();

			if (script)
				delete script;

			script = nullptr;
		}
	}
}

void Console::update(unsigned int, World &) {
	if (opened) {
		// get correct prompt
		switch (status) {
			case 2:
				prompt = ">> " + command + "_";
				break;

			case 0:
			case 1:
			default:
				if (result.empty())
					prompt = "> " + command + "_";
				else
					prompt = result;
				break;
		}

		// draw text
		Text::get_instance().destroy(text);
		text = Text::get_instance().write(font, prompt, color);

		// calculate size
		size.w = Engine::get_instance().get_viewport().get_width() - 2*padding_left;
		size.h = text->h + 2*padding_text;

		size.x = padding_left;
		size.y = Engine::get_instance().get_viewport().get_height() - size.h - padding_bottom - padding_text;
	}
}

void Console::draw(const Viewport &) const {
	if (opened) {
		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), box.r, box.g, box.b, box.a);
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &size);

		// draw text
		Text::get_instance().render(Context::get_instance().get_renderer(), text, size.x + padding_text, size.y + padding_text, 0);
	}
}

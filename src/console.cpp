#include <SDL.h>

#include "context.h"
#include "script.h"
#include "spec.h"
#include "text.h"
#include "viewport.h"

#include "console.h"

Console & Console::get_instance() {
	static Console console;
	return console;
}

Console::Console() : Drawable("console", "console", Vector2f{0, 0}, 0, Vector2f{0, 0}, 1, 9002), opened(false), command(""), result(""), surface(nullptr), padding_bottom(4), padding_left(5), padding_font(2) {
}

void Console::dispatch(const SDL_Event & event) {
	if (opened) {
		// grab keyboard focus
		if (!Input::get_instance().check("console"))
			Input::get_instance().grab("console");

		if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
			if (event.key.keysym.sym == SDLK_BACKQUOTE) {
				// stop text input
				opened = false;
				SDL_StopTextInput();
				command.clear();
				result.clear();
			}
			else if (event.key.keysym.sym == SDLK_BACKSPACE) {
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
					try {
						Script script;
						script.load();
						script.add_script(command);
						result = script.get_result();
					}
					catch (std::runtime_error & err) {
						std::string str = err.what();
						result = "> lua error" + str.substr(str.rfind(":"));
					}

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
		if (Input::get_instance().check("console"))
			Input::get_instance().release("console");

		if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
			if (event.key.keysym.sym == SDLK_BACKQUOTE) {
				// open console and start text input so we get TextInput events
				opened = true;
				SDL_StartTextInput();

				// grab input
				Input::get_instance().grab("console");
			}
		}
	}
}

void Console::draw(const Viewport & viewport) const {
	if (opened) {
		// drawing rectable
		SDL_Rect rect = {padding_left, viewport.get_height() - Text::get_instance().get_size() - padding_bottom - padding_font, viewport.get_width() - padding_left*2, Text::get_instance().get_size() + padding_font*2};

		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), Spec::get_instance().get_int("console/box/r"), Spec::get_instance().get_int("console/box/g"), Spec::get_instance().get_int("console/box/b"), Spec::get_instance().get_int("console/box/a"));
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &rect);

		// draw text
		SDL_Color color = {static_cast<Uint8>(Spec::get_instance().get_int("console/text/r")), static_cast<Uint8>(Spec::get_instance().get_int("console/text/g")), static_cast<Uint8>(Spec::get_instance().get_int("console/text/b")), 255};
		if (result.empty())
			Text::get_instance().write(Context::get_instance().get_renderer(), "> " + command + "_", rect.x + padding_font, rect.y + padding_font, color);
		else
			Text::get_instance().write(Context::get_instance().get_renderer(), result, rect.x + padding_font, rect.y + padding_font, color);
	}
}

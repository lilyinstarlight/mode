#include "context.h"
#include "script.h"
#include "spec.h"
#include "text.h"

#include "console.h"

Console::Console() : Drawable("console", "console", Vector2f(0, 0), 0, Vector2f(0, 0), 1, 9003), opened(false), command(""), result(""), surface(nullptr), padding_bottom(4), padding_left(5), padding_font(2) {}

Console::Console(const Console & console) : Drawable(console), opened(console.opened), command(console.command), result(console.result), surface(nullptr), padding_bottom(console.padding_bottom), padding_left(console.padding_left), padding_font(console.padding_font) {}

void Console::dispatch(const SDL_Event & event) {
	if (opened) {
		// grab keyboard focus
		if (!Input::get_instance().check("console")) {
			Input::get_instance().grab("console");
			SDL_StartTextInput();
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
		else if (event.type == SDL_TEXTINPUT) {
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
			Text::get_instance().write(Context::get_instance().get_renderer(), "> " + command + "█", rect.x + padding_font, rect.y + padding_font, color);
		else
			Text::get_instance().write(Context::get_instance().get_renderer(), result, rect.x + padding_font, rect.y + padding_font, color);
	}
}

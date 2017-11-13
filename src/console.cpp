#include "spec.h"
#include "text.h"
#include "viewport.h"

#include "console.h"

Console & Console::get_instance() {
	static Console console;
	return console;
}

Console::Console() : Drawable("name", Vector2f{0, 0}, 0, Vector2f{0, 0}, 1), open(false), command(""), surface(nullptr), padding_bottom(4), padding_left(5), padding_font(2) {
}

void Console::draw(const Viewport & viewport) const {
	SDL_Rect rect = {padding_left, viewport.get_height() - Text::get_instance().get_size() - padding_bottom - padding_font, viewport.get_width() - padding_left*2, Text::get_instance().get_size() + padding_font*2};
	(void)rect;

	if (open) {
		// TODO: draw a semi-transparent rectangle with text and a cursor
	}
}

void Console::update(unsigned int) {
	if (open) {
		if (!Input::get_instance().check("console"))
			Input::get_instance().grab("console");

		// TODO: get keypresses and add them to command
	}
	else {
		// TODO: check for console keypress
	}
}

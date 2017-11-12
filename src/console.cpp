#include "spec.h"

#include "console.h"

Console & Console::get_instance() {
	static Console console;
	return console;
}

Console::Console() : Drawable("name", Vector2f{-1, -1}, 0, Vector2f{-1, -1}, 1), open(false), command(""), surface(nullptr), padding_bottom(4), padding_left(5), padding_font(2), size{-1, -1, -1, -1} {
}

void Console::draw() const {
	if (open) {
		// TODO: draw a semi-transparent rectangle with text and a cursor
	}
}

void Console::update(unsigned int) {
	if (open) {
		size.x = padding_left;
		size.y = Spec::get_instance().get_int("view/height") - Spec::get_instance().get_int("font/size") - padding_bottom - padding_font;
		size.w = Spec::get_instance().get_int("view/width") - padding_left*2;
		size.h = Spec::get_instance().get_int("font/size") + padding_font*2;

		if (!Input::get_instance().check("console"))
			Input::get_instance().grab("console");

		// TODO: get keypresses and add them to command
	}
	else {
		size.x = -1;
		size.y = -1;
		size.w = -1;
		size.h = -1;

		// TODO: check for console keypress
	}
}

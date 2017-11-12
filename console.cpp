#include "console.h"

Console & Console::get_instance() {
	static Console console;
	return console;
}

Console::Console() : open(false) {
}

void Console::draw() const {
	if (open) {
		// TODO: draw a semi-transparent rectangle with text and a cursor
	}
}

void Console::update(unsigned int ticks) {
	if (open) {
		if (!input.check("console"))
			input.grab("console");

		// TODO: get keypresses and add them to command
	}
	else {
		// TODO: check for console keypress
	}
}

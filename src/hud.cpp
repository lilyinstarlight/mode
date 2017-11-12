#include "hud.h"

HUD & HUD::get_instance() {
	static HUD hud;
	return hud;
}

HUD::HUD() : first(true), open(false), surface(nullptr), padding_top(20), padding_left(20), size{padding_top, padding_left, Spec::get_instance().get_int("hud/width"), Spec::get_instance().get_int("hud/height")} {
}

void Console::draw() const {
	if (open) {
		// TODO: draw a semi-transparent rectangle with dynamic text
	}
}

void Console::update(unsigned int ticks) {
	// TODO: toggle hud on keypress
}

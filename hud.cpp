#include "hud.h"

HUD & HUD::get_instance() {
	static HUD hud;
	return hud;
}

HUD::HUD() : first(true), open(false) {
}

void Console::draw() const {
	if (open) {
		// TODO: draw a semi-transparent rectangle with dynamic text
	}
}

void Console::update(unsigned int ticks) {
	// TODO: toggle hud on keypress
}

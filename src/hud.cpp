#include "spec.h"

#include "hud.h"

HUD & HUD::get_instance() {
	static HUD hud;
	return hud;
}

HUD::HUD() : Drawable("hud", Vector2f{0, 0}, 0, Vector2f{0, 0}, 1), first(true), open(false), surface(nullptr), padding_top(20), padding_left(20), size{padding_top, padding_left, Spec::get_instance().get_int("hud/width"), Spec::get_instance().get_int("hud/height")} {
}

void HUD::draw(const Viewport &) const {
	if (first || open) {
		// TODO: draw a semi-transparent rectangle with dynamic text
	}
}

void HUD::update(unsigned int) {
	// TODO: toggle hud on keypress
}

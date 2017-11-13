#include "spec.h"

#include "hud.h"

HUD & HUD::get_instance() {
	static HUD hud;
	return hud;
}

HUD::HUD() : Drawable("hud", Vector2f{0, 0}, 0, Vector2f{0, 0}, 1), first(1500), open(false), surface(nullptr), padding_top(20), padding_left(20), padding_font(4), size{padding_top, padding_left, Spec::get_instance().get_int("hud/width"), Spec::get_instance().get_int("hud/height")} {
}

void HUD::draw(const Viewport &) const {
	if (first || open) {
		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), Spec::get_instance().get_int("hud/r"), Spec::get_instance().get_int("hud/g"), Spec::get_instance().get_int("hud/b"), Spec::get_instance().get_int("hud/a"))
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &rect);

		// draw text
		Text::get_instance().write(Context::get_instance().get_renderer(), Spec::get_instance().get_str("hud/str"), size.x + padding_font, size.y + padding_font);
	}
}

void HUD::update(unsigned int ticks) {
	if (first > 0)
		--first;

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_H) {
			open = !open;
	}
}

#include "context.h"
#include "spec.h"
#include "text.h"

#include "hud.h"

HUD & HUD::get_instance() {
	static HUD hud;
	return hud;
}

HUD::HUD() : Drawable("hud", Vector2f{0, 0}, 0, Vector2f{0, 0}, 1, 9001), initial(1500), opened(false), surface(nullptr), padding_top(20), padding_left(20), padding_font(4), size{padding_top, padding_left, Spec::get_instance().get_int("hud/width"), Spec::get_instance().get_int("hud/height")}, str(Spec::get_instance().get_str("hud/str")) {
}

void HUD::dispatch(const SDL_Event & event) {
	if (event.type == SDL_KEYDOWN) {
		// open hud on h
		if (event.key.keysym.sym == SDLK_h)
			opened = !opened;
	}
}

void HUD::draw(const Viewport &) const {
	if (initial > 0 || opened) {
		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), Spec::get_instance().get_int("hud/box/r"), Spec::get_instance().get_int("hud/box/g"), Spec::get_instance().get_int("hud/box/b"), Spec::get_instance().get_int("hud/box/a"));
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &size);

		// draw text
		SDL_Color color = {static_cast<Uint8>(Spec::get_instance().get_int("hud/text/r")), static_cast<Uint8>(Spec::get_instance().get_int("hud/text/g")), static_cast<Uint8>(Spec::get_instance().get_int("hud/text/b")), 255};
		Text::get_instance().write(Context::get_instance().get_renderer(), str, size.x + padding_font, size.y + padding_font, color);
	}
}

void HUD::update(unsigned int ticks) {
	// decrement initial counter on whether this is shown at the beginning of game or not
	if (initial > 0)
		initial -= ticks;
}

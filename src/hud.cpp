#include "context.h"
#include "spec.h"
#include "text.h"

#include "hud.h"

HUD & HUD::get_instance() {
	static HUD hud;
	return hud;
}

HUD::HUD() : Drawable("hud", Vector2f{0, 0}, 0, Vector2f{0, 0}, 1, 9001), first(1500), open(false), surface(nullptr), padding_top(20), padding_left(20), padding_font(4), border_thickness(4), size{padding_top, padding_left, Spec::get_instance().get_int("hud/width"), Spec::get_instance().get_int("hud/height")}, border{size.x - border_thickness, size.y - border_thickness, size.w + border_thickness, size.h + border_thickness} {
}

void HUD::draw(const Viewport &) const {
	if (first || open) {
		// draw border
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), Spec::get_instance().get_int("hud/border/r"), Spec::get_instance().get_int("hud/border/g"), Spec::get_instance().get_int("hud/border/b"), Spec::get_instance().get_int("hud/border/a"));
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &border);

		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), Spec::get_instance().get_int("hud/box/r"), Spec::get_instance().get_int("hud/box/g"), Spec::get_instance().get_int("hud/box/b"), Spec::get_instance().get_int("hud/box/a"));
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &size);

		// draw text
		SDL_Color color = {(Uint8)Spec::get_instance().get_int("hud/text/r"), (Uint8)Spec::get_instance().get_int("hud/text/g"), (Uint8)Spec::get_instance().get_int("hud/text/b"), 255};
		Text::get_instance().write(Context::get_instance().get_renderer(), Spec::get_instance().get_str("hud/str"), size.x + padding_font, size.y + padding_font, color);
	}
}

void HUD::update(unsigned int ticks) {
	const SDL_Event * event = Input::get_instance().get_event();

	if (first > 0)
		first -= ticks;

	if (event->type == SDL_KEYDOWN) {
		if (event->key.keysym.sym == SDLK_h)
			open = !open;
	}
}

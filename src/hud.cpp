#include "context.h"
#include "spec.h"
#include "text.h"

#include "hud.h"

HUD & HUD::get_instance() {
	static HUD hud;
	return hud;
}

HUD::HUD() : Drawable("hud", Vector2f{0, 0}, 0, Vector2f{0, 0}, 1, 9001), initial(1500), opened(false), surface(nullptr), padding_top(20), padding_left(20), padding_text(4), str(Spec::get_instance().get_str("hud/str")) {
}

void HUD::dispatch(const SDL_Event &) {}

void HUD::draw(const Viewport &) const {
	if (initial > 0 || opened) {
		SDL_Color color = {static_cast<Uint8>(Spec::get_instance().get_int("hud/text/r")), static_cast<Uint8>(Spec::get_instance().get_int("hud/text/g")), static_cast<Uint8>(Spec::get_instance().get_int("hud/text/b")), 255};

		// write text
		SDL_Surface * text = Text::get_instance().write(str, color);
		SDL_Rect size = {padding_top, padding_left, text->w + 2*padding_text, text->h + 2*padding_text};

		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), Spec::get_instance().get_int("hud/box/r"), Spec::get_instance().get_int("hud/box/g"), Spec::get_instance().get_int("hud/box/b"), Spec::get_instance().get_int("hud/box/a"));
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &size);

		// draw text
		Text::get_instance().render(Context::get_instance().get_renderer(), text, size.x + padding_text, size.y + padding_text);
	}
}

void HUD::update(unsigned int ticks) {
	// decrement initial counter on whether this is shown at the beginning of game or not
	if (initial > 0)
		initial -= ticks;
}

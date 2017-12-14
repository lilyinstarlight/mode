#include "context.h"
#include "spec.h"
#include "text.h"
#include "viewport.h"

#include "dialog.h"

Dialog::Dialog(const std::string & name, const std::string & text, bool top, bool open) : Drawable(name,
			Spec::get_instance().get_str(name + "/type"),
			Vector2f(Spec::get_instance().get_int(name + "/position/x"),
					 Spec::get_instance().get_int(name + "/position/y")),
			Spec::get_instance().get_int(name + "/rotation"),
			Vector2f(0, 0), 1, 9001),
		above(top),
		opened(open),
		padding_text(4),
		str(text == "" ? Spec::get_instance().get_str(name + "/str") : text),
		color{static_cast<Uint8>(Spec::get_instance().get_int(name + "/text/r")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/text/g")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/text/b")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/text/a"))},
		box{static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/r")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/g")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/b")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/a"))} {}

Dialog::Dialog(const Dialog & dialog) : Drawable(dialog), above(dialog.above), opened(dialog.opened), padding_text(dialog.padding_text), str(dialog.str), color(dialog.color), box(dialog.box) {}

void Dialog::draw(const Viewport & viewport) const {
	if (opened) {
		// write text
		SDL_Surface * text = Text::get_instance().write(str, color);
		SDL_Rect size;

		// calculate size
		if (above) {
			size.x = get_x();
			size.y = get_y();
		}
		else {
			size.x = get_x() - viewport.get_x();
			size.y = get_y() - viewport.get_y();
		}

		size.w = text->w + 2*padding_text;
		size.h = text->h + 2*padding_text;

		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), box.r, box.g, box.b, box.a);
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &size);

		// draw text
		Text::get_instance().render(Context::get_instance().get_renderer(), text, size.x + padding_text, size.y + padding_text);
	}
}

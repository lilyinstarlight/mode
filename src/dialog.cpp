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
		box{static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/r")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/g")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/b")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/a"))},
		text(Text::get_instance().write(str, color)),
		size{0, 0, 0, 0} {}

Dialog::Dialog(const Dialog & dialog) : Drawable(dialog), above(dialog.above), opened(dialog.opened), padding_text(dialog.padding_text), str(dialog.str), color(dialog.color), box(dialog.box), text(dialog.text), size(dialog.size) {}

void Dialog::update(unsigned int, World &) {
	if (opened) {
		// recreate text surface
		Text::get_instance().destroy(text);
		text = Text::get_instance().write(str, color);

		// calculate size
		size.x = get_x();
		size.y = get_y();

		size.w = text->w + 2*padding_text;
		size.h = text->h + 2*padding_text;
	}
}

void Dialog::draw(const Viewport & viewport) const {
	if (opened) {
		SDL_Rect dst = size;

		if (!above) {
			dst.x = size.x - viewport.get_x();
			dst.y = size.y - viewport.get_y();
		}

		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), box.r, box.g, box.b, box.a);
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &dst);


		// draw text
		Text::get_instance().render(Context::get_instance().get_renderer(), text, dst.x + padding_text, dst.y + padding_text, get_rotation());
	}
}

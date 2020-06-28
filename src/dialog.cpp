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
		_above(top),
		_opened(open),
		_padding_text(4),
		_str(text == "" ? Spec::get_instance().get_str(name + "/str") : text),
		_font(Spec::get_instance().check(name + "/font") ? name + "/font" : "font/default"),
		_color{static_cast<Uint8>(Spec::get_instance().get_int(name + "/text/r")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/text/g")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/text/b")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/text/a"))},
		_box{static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/r")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/g")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/b")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/box/a"))},
		_border{static_cast<Uint8>(Spec::get_instance().get_int(name + "/border/r")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/border/g")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/border/b")), static_cast<Uint8>(Spec::get_instance().get_int(name + "/border/a"))},
		_border_width(Spec::get_instance().get_int(name + "/border/width")),
		_text(Text::get_instance().write(_font, _str, _color)),
		_size{0, 0, 0, 0} {}

Dialog::Dialog(const Dialog & dialog) : Drawable(dialog), _above(dialog._above), _opened(dialog._opened), _padding_text(dialog._padding_text), _str(dialog._str), _font(dialog._font), _color(dialog._color), _box(dialog._box), _border(dialog._border), _border_width(dialog._border_width), _text(Text::get_instance().write(_font, _str, _color)), _size(dialog._size) {}

void Dialog::update(unsigned int, World &) {
	if (_opened) {
		// recreate text surface
		Text::get_instance().destroy(_text);
		_text = Text::get_instance().write(_font, _str, _color);

		// calculate size
		_size.x = get_x();
		_size.y = get_y();

		_size.w = _text->w + 2*_padding_text;
		_size.h = _text->h + 2*_padding_text;
	}
}

void Dialog::draw(const Viewport & viewport) const {
	if (_opened) {
		SDL_Rect dst = _size;
		SDL_Rect bdr = _size;

		bdr.x -= _border_width;
		bdr.w += 2*_border_width;
		bdr.y -= _border_width;
		bdr.h += 2*_border_width;

		if (!_above) {
			dst.x = dst.x - viewport.get_x();
			dst.y = dst.y - viewport.get_y();

			bdr.x = bdr.x - viewport.get_x();
			bdr.y = bdr.y - viewport.get_y();
		}

		// draw border
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), _border.r, _border.g, _border.b, _border.a);
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &bdr);

		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), _box.r, _box.g, _box.b, _box.a);
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &dst);

		// draw text
		Text::get_instance().render(Context::get_instance().get_renderer(), _text, dst.x + _padding_text, dst.y + _padding_text, get_rotation());
	}
}

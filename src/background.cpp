#include "imagefactory.h"

#include "background.h"

Background::Background(const std::string & name) : Drawable(name, Vector2f(0, 0), 0, Vector2f(0, 0), 1, Spec::get_instance().get_int(name + "/index")), factor(Spec::get_instance().get_float(name + "/factor")), tile(Spec::get_instance().get_bool(name + "/tile")), image(ImageFactory::get_instance().get_image(name)) {}

Background::Background(const Background & b) : Drawable(b), factor(b.factor), tile(b.tile), image(b.image) {}

void Background::draw(const Viewport & viewport) const {
	if (tile) {
		// iterate over tiles in map
		for (int x = static_cast<int>(get_x()*factor - viewport.get_x()*factor) % get_width() - get_width(); x < viewport.get_width(); x += get_width()) {
			for (int y = static_cast<int>(get_y()*factor - viewport.get_y()*factor) % get_height() - get_height(); y < viewport.get_height(); y += get_height()) {
				// draw adjusted tile
				SDL_Rect src = {0, 0, get_width(), get_height()};
				SDL_Rect dst = {x, y, src.w, src.h};
				image->draw(src, dst, 0);
			}
		}
	}
	else {
		// draw adjusted background
		SDL_Rect src = {0, 0, get_width(), get_height()};
		SDL_Rect dst = {static_cast<int>(get_x()*factor - viewport.get_x()*factor), static_cast<int>(get_y()*factor - viewport.get_y()*factor), src.w, src.h};
		image->draw(src, dst, 0);
	}
}

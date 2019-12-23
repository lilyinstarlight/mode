#include <stdexcept>

#include "imagefactory.h"

#include "background.h"

Background::Background(const std::string & name) : Drawable(name,
		Spec::get_instance().get_str(name + "/type"),
		Vector2f(Spec::get_instance().get_int(name + "/position/x"),
		         Spec::get_instance().get_int(name + "/position/y")),
		0, Vector2f(0, 0), 1, Spec::get_instance().get_int(name + "/index")
	),
	factor(Spec::get_instance().get_float(name + "/factor")),
	tile(NONE),
	image(ImageFactory::get_instance().get_image(name)) {
	std::string tiling = Spec::get_instance().get_str(name + "/tile");
	if (tiling == "none")
		tile = NONE;
	else if (tiling == "x")
		tile = X;
	else if (tiling == "y")
		tile = Y;
	else if (tiling == "both")
		tile = BOTH;
	else
		throw std::runtime_error("Invalid tiling: " + Spec::get_instance().get_str(get_name() + "/tile"));
}

Background::Background(const Background & b) : Drawable(b), factor(b.factor), tile(b.tile), image(b.image) {}

void Background::draw(const Viewport & viewport) const {
	SDL_Rect src = {0, 0, image->get_width(), image->get_height()};
	SDL_Rect dst = {static_cast<int>(get_x()*factor - viewport.get_x()*factor), static_cast<int>(get_y()*factor - viewport.get_y()*factor), src.w, src.h};

	switch (tile) {
		case NONE:
			// draw adjusted background
			image->draw(src, dst, 0);
			break;

		case X:
			// iterate over tiles in map
			for (int x = static_cast<int>(get_x()*factor - viewport.get_x()*factor) % image->get_width() - image->get_width(); x < viewport.get_width(); x += image->get_width()) {
				// draw adjusted tile
				dst.x = x;
				image->draw(src, dst, 0);
			}

			break;

		case Y:
			// iterate over tiles in map
			for (int y = static_cast<int>(get_y()*factor - viewport.get_y()*factor) % image->get_height() - image->get_height(); y < viewport.get_height(); y += image->get_height()) {
				// draw adjusted tile
				dst.y = y;
				image->draw(src, dst, 0);
			}

			break;

		case BOTH:
			// iterate over tiles in map
			for (int x = static_cast<int>(get_x()*factor - viewport.get_x()*factor) % image->get_width() - image->get_width(); x < viewport.get_width(); x += image->get_width()) {
				for (int y = static_cast<int>(get_y()*factor - viewport.get_y()*factor) % image->get_height() - image->get_height(); y < viewport.get_height(); y += image->get_height()) {
					// draw adjusted tile
					dst.x = x;
					dst.y = y;
					image->draw(src, dst, 0);
				}
			}

			break;
	}
}

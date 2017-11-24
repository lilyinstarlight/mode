#include "imagefactory.h"

#include "background.h"

Background::Background(const std::string & name) : Drawable(name, Vector2f(0, 0), 0, Vector2f(0, 0), 1, Spec::get_instance().get_int(name + "/index")), width(Spec::get_instance().get_int(name + "/width")), height(Spec::get_instance().get_int(name + "/height")), factor(Spec::get_instance().get_float(name + "/factor")), tile(Spec::get_instance().get_bool(name + "/tile")), image(ImageFactory::get_instance().get_image(name)) {}

Background::Background(const Background & b) : Drawable(b), width(b.width), height(b.height), factor(b.factor), tile(b.tile), image(b.image) {}

void Background::draw(const Viewport & viewport) const {
	int x = viewport.get_x() - get_x();
	int y = viewport.get_y() - get_y();

	// draw adjusted background
	SDL_Rect src = {x, y, get_width() - x, get_height() - y};
	SDL_Rect dst = {0, 0, src.w, src.h};
	image->draw(src, dst);

	// draw tiled background
	if (tile) {
		src.x = 0;
		src.w = viewport.get_width() - x;
		dst.x = get_width() - x;
		image->draw(src, dst);
	}
}

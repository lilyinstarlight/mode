#include "imagefactory.h"

#include "background.h"

Background::Background(const std::string & name) : Drawable(name, Vector2f(0, 0), 0, Vector2f(0, 0), 1, Spec::get_instance().get_int(name + "/index")), width(Spec::get_instance().get_int(name + "/width")), height(Spec::get_instance().get_int(name + "/height")), factor(Spec::get_instance().get_float(name + "/factor")), image(ImageFactory::get_instance().get_image(name)) {}

void Background::update(unsigned int) {
	// move background according to factor
	set_x(static_cast<int>(get_x() / factor) % image->get_width());
}

void Background::draw(const Viewport & viewport) const {
	// draw background across viewport
	image->draw(viewport, 0, 0, -get_x(), -get_y());
	image->draw(viewport, 0, 0, image->get_width() - get_x(), -get_y());
	image->draw(viewport, 0, 0, -get_x(), image->get_height() - get_y());
}


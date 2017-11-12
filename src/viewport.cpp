#include "spec.h"

#include "viewport.h"

Viewport::Viewport(const World & w) :
	world(w),
	position(0, 0),
	width(Spec::get_instance().get_int("view/width")),
	height(Spec::get_instance().get_int("view/height")),
	tracking(nullptr) {}

void Viewport::draw() const {
}

void Viewport::update(unsigned int ticks) {
	const float x = tracking->get_x();
	const float y = tracking->get_y();

	position[0] = (x + obj->get_width()/2) - width/2;
	position[1] = (y + obj->get_height/2) - height/2;

	if (position[0] < 0)
		position[0] = 0;
	else if (position[0] > world.get_width() - width) {
		position[0] = world.get_width() - width;
	}

	if (position[1] < 0)
		position[1] = 0;
	else if (position[1] > world.get_height() - height) {
		position[1] = (world.get_height() - height);
	}
}

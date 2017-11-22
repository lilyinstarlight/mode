#include "spec.h"

#include "viewport.h"

Viewport::Viewport(const World & w) :
	world(w),
	position(0, 0),
	width(Spec::get_instance().get_int("view/width")),
	height(Spec::get_instance().get_int("view/height")),
	tracking(&world.get_player()) {}

void Viewport::update(unsigned int) {
	// track object
	const float x = tracking->get_x();
	const float y = tracking->get_y();

	// set position based on tracked object
	position[0] = (x + tracking->get_width()/2) - width/2;
	position[1] = (y + tracking->get_height()/2) - height/2;

	// clamp to world boundaries
	if (position[0] < 0)
		position[0] = 0;
	else if (position[0] > world.get_width() - width) {
		position[0] = world.get_width() - width;
	}

	// clamp to world boundaries
	if (position[1] < 0)
		position[1] = 0;
	else if (position[1] > world.get_height() - height) {
		position[1] = (world.get_height() - height);
	}
}

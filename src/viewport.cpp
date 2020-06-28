#include "spec.h"

#include "viewport.h"

Viewport::Viewport() :
	_position(0, 0),
	_width(Spec::get_instance().get_int("view/width")),
	_height(Spec::get_instance().get_int("view/height")),
	_tracking(nullptr) {}

void Viewport::update(unsigned int, World & world) {
	// bail if no object to track
	if (!_tracking)
		return;

	if (!world.check(_tracking)) {
		_tracking = nullptr;
		return;
	}

	// track object
	const float x = _tracking->get_x();
	const float y = _tracking->get_y();

	// set position based on tracked object
	_position[0] = (x + _tracking->get_width()/2) - _width/2;
	_position[1] = (y + _tracking->get_height()/2) - _height/2;

	// clamp to world boundaries
	if (_position[0] < 0)
		_position[0] = 0;
	else if (_position[0] > world.get_width() - _width) {
		_position[0] = world.get_width() - _width;
	}

	// clamp to world boundaries
	if (_position[1] < 0)
		_position[1] = 0;
	else if (_position[1] > world.get_height() - _height) {
		_position[1] = (world.get_height() - _height);
	}
}

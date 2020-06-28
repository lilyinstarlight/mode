#include <SDL.h>

#include "spec.h"
#include "world.h"

#include "body.h"

Body::Body(const std::string & name, bool fixed) : Sprite(name), _hardness(Spec::get_instance().get_float(name + "/hardness")), _elasticity(Spec::get_instance().get_float(name + "/elasticity")), _fixed(fixed) {}

Body::Body(const Body & b) : Sprite(b), _hardness(b._hardness), _elasticity(b._elasticity), _fixed(b._fixed) {}

void Body::update(unsigned int ticks, World & world) {
	// apply basic rigid body mechanics for each sprite
	for (Body * body : world.get_all<Body>()) {
		// ignore self and fixed bodies
		if (body == this || body->is_fixed())
			continue;

		// get collision area
		SDL_Rect area = get_collision_strategy().intersection(*this, *body);

		// get collision normal
		Vector2f normal = get_collision_strategy().get(*this, *body);

		// check for collision
		if (normal.get_x() == 0 && normal.get_y() == 0)
			continue;

		// set velocity to normal
		body->set_velocity_x(body->get_velocity_x()/(std::abs(area.w*normal.get_x())*_hardness + 1) + _elasticity*area.w*normal.get_x());
		body->set_velocity_y(body->get_velocity_y()/(std::abs(area.h*normal.get_y())*_hardness + 1) + _elasticity*area.h*normal.get_y());
	}

	Sprite::update(ticks, world);
}

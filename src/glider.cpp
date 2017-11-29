#include "util.h"

#include "glider.h"

Glider::Glider() : Sprite("glider"), alive(false), origin{0, 0}, max(120.0) {}

void Glider::update(unsigned int ticks) {
	Sprite::update(ticks);

	if (Util::distance(origin, get_position()) > max)
		alive = false;
}

void Glider::create() {
	alive = true;

	origin = get_position();
}

void Glider::destroy() {
	alive = false;
}

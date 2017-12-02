#include <cmath>

#include "input.h"
#include "spec.h"
#include "world.h"

#include "player.h"

Player::Player() : Sprite("player"), gliders("glider", Spec::get_instance().get_int("player/glider/initial")), hp(Spec::get_instance().get_int("player/hp")), shot(false) {
	Input::get_instance().grab("player");
}

Player::~Player() {
	Input::get_instance().release("player");
}

void Player::update(unsigned int ticks, World & world) {
	Sprite::update(ticks, world);

	if (shot) {
		Projectile & glider = gliders.create();
		world.add(glider);

		for (Sprite * observer : get_observers())
			glider.observe(*observer);

		glider.set_y(get_y() + get_height()/2 + Spec::get_instance().get_int("player/glider/offset/y"));

		int speed = Spec::get_instance().get_int("player/glider/speed") + std::abs(get_velocity_x());

		if (get_direction() == "left") {
			glider.set_x(get_x() - glider.get_width() - Spec::get_instance().get_int("player/glider/offset/x"));
			glider.set_velocity_x(-speed);
		}
		else {
			glider.set_x(get_x() + get_width() + glider.get_width() + Spec::get_instance().get_int("player/glider/offset/x"));
			glider.set_velocity_x(speed);
		}

		glider.set_origin();

		shot = false;
	}
}

void Player::shoot() {
	shot = true;
}

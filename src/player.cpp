#include <cmath>

#include "input.h"
#include "spec.h"

#include "player.h"

Player::Player() : Sprite("player"), gliders("glider"), hp(64) {
	Input::get_instance().grab("player");
}

Player::~Player() {
	Input::get_instance().release("player");
}

void Player::dispatch(const SDL_Event & event) {
	Sprite::dispatch(event);
	gliders.dispatch(event);
}

void Player::draw(const Viewport & viewport) const {
	Sprite::draw(viewport);
	gliders.draw(viewport);
}

void Player::update(unsigned int ticks) {
	Sprite::update(ticks);
	gliders.update(ticks);
}

void Player::shoot() {
	Projectile & glider = gliders.create();

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
}

#include "input.h"

#include "player.h"

Player::Player(const World & w) : Sprite("player", w, true), hp(64) {
	script = new Script("player", *this);

	Input::get_instance().grab("player");
}

void Player::update(unsigned int) {
	if (Input::get_instance().check("player")) {
		// TODO: get keypresses and move sprite
	}
}

void Player::inject(const Sprite &) {
}

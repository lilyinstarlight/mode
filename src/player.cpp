#include "player.h"

Player::Player(const World & w) : Sprite("player", w, true) {
	script = new Script("player", *this);

	input.grab("player");
}

void Player::update(unsigned int ticks) {
	if (input.check("player")) {
		// TODO: get keypresses and move sprite
	}
}

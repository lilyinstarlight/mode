#include "player.h"

Player::Player(const World & w) : Sprite("player", w, true), observers{}, observer_interval(500), observer_timer(0) {
	script = new Script("player", *this);

	input.grab("player");
}

void Player::update(unsigned int ticks) {
	if (input.check("player")) {
		// TODO: get keypresses and move sprite

		observer_timer += ticks;
		if (observer_timer > observer_interval) {
			for (const Observer & observer : observers)
				observer.signal(*this);

			observer_timer = 0;
		}
	}
}

void Player::observe(const Observer & observer) {
	observers.push_back(observer);
}

void Player::ignore(const Observer & observer) {
	observers.remove(observer);
}

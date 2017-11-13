#include <SDL.h>

#include "input.h"

#include "player.h"

Player::Player(const World & w) : Sprite("player", w, false), hp(64) {
	script = new Script("player", *this);

	Input::get_instance().grab("player");
}

Player::~Player() {
	Input::get_instance().release("player");
}

void Player::update(unsigned int) {
	if (Input::get_instance().check("player")) {
		const Uint8 * keystate = Input::get_instance().get_keystate();

		if (keystate[SDL_SCANCODE_A])
			set_velocity_x(get_velocity_x() - 1.0f);
		if (keystate[SDL_SCANCODE_D])
			set_velocity_x(get_velocity_x() + 1.0f);
		if (keystate[SDL_SCANCODE_W])
			set_velocity_y(get_velocity_y() - 1.0f);
		if (keystate[SDL_SCANCODE_S])
			set_velocity_y(get_velocity_y() + 1.0f);
	}
}

void Player::inject(const Sprite &) {
}

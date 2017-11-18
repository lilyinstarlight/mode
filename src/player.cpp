#include <SDL.h>

#include "input.h"

#include "player.h"

Player::Player() : Sprite("player"), hp(64) {
	Input::get_instance().grab("player");
}

Player::~Player() {
	Input::get_instance().release("player");
}

void Player::update(unsigned int ticks) {
	// check for input focus
	if (Input::get_instance().check("player")) {
		const Uint8 * keystate = Input::get_instance().get_keystate();

		// stop velocity
		set_velocity_x(0.0f);
		set_velocity_y(0.0f);

		// add wasd controls
		if (keystate[SDL_SCANCODE_A])
			set_velocity_x(get_velocity_x() - 1.0f);
		if (keystate[SDL_SCANCODE_D])
			set_velocity_x(get_velocity_x() + 1.0f);
		if (keystate[SDL_SCANCODE_W])
			set_velocity_y(get_velocity_y() - 1.0f);
		if (keystate[SDL_SCANCODE_S])
			set_velocity_y(get_velocity_y() + 1.0f);
	}

	Sprite::update(ticks);
}

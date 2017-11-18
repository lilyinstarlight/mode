#include <SDL.h>

#include "input.h"
#include "spec.h"

#include "player.h"

Player::Player() : Sprite("player"), hp(64) {
	Input::get_instance().grab("player");
}

Player::Player(const Player & player) : Sprite("player"), hp(player.hp) {}

Player::~Player() {
	Input::get_instance().release("player");
}

void Player::update(unsigned int ticks) {
	// check for input focus
	if (Input::get_instance().check("player")) {
		const Uint8 * keystate = Input::get_instance().get_keystate();

		// stop movement
		Vector2f movement(0.0f, 0.0f);

		// add wasd controls
		if (keystate[SDL_SCANCODE_A])
			movement[0] -= Spec::get_instance().get_float("player/speed/left");
		if (keystate[SDL_SCANCODE_D])
			movement[0] += Spec::get_instance().get_float("player/speed/right");
		if (keystate[SDL_SCANCODE_W])
			movement[1] -= Spec::get_instance().get_float("player/speed/up");
		if (keystate[SDL_SCANCODE_S])
			movement[1] += Spec::get_instance().get_float("player/speed/down");

		// add movement to velocity
		set_velocity_x(movement[0]);
		set_velocity_y(movement[1]);
	}

	Sprite::update(ticks);
}

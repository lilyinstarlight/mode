#include "input.h"

#include "player.h"

Player::Player() : Sprite("player"), hp(64) {
	Input::get_instance().grab("player");
}

Player::~Player() {
	Input::get_instance().release("player");
}

#include <cmath>

#include "input.h"
#include "spec.h"
#include "world.h"

#include "player.h"

Player::Player() : Sprite("player"), shooting(Spec::get_instance().check("player/projectile/name")), projectiles(shooting ? Spec::get_instance().get_str("player/projectile/name") : "", shooting ? Spec::get_instance().get_int("player/projectile/initial") : 0), hp(Spec::get_instance().check("player/hp") ? Spec::get_instance().get_int("player/hp") : 1), shot(false) {
	Input::get_instance().grab("player");
}

Player::~Player() {
	Input::get_instance().release("player");
}

void Player::update(unsigned int ticks, World & world) {
	Sprite::update(ticks, world);

	if (shot) {
		Projectile & projectile = projectiles.create();
		world.add(projectile);

		for (Sprite * observer : get_observers())
			projectile.observe(*observer);

		projectile.set_y(get_y() + get_height()/2 + Spec::get_instance().get_int("player/projectile/offset/y"));

		int speed = Spec::get_instance().get_int("player/projectile/speed") + std::abs(get_velocity_x());

		if (get_direction() == "left") {
			projectile.set_x(get_x() - projectile.get_width() - Spec::get_instance().get_int("player/projectile/offset/x"));
			projectile.set_velocity_x(-speed);
		}
		else {
			projectile.set_x(get_x() + get_width() + projectile.get_width() + Spec::get_instance().get_int("player/projectile/offset/x"));
			projectile.set_velocity_x(speed);
		}

		projectile.set_origin();

		shot = false;
	}
}

void Player::shoot() {
	if (shooting)
		shot = true;
}

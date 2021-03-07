#include <cmath>

#include "input.h"
#include "spec.h"
#include "world.h"

#include "player.h"

Player::Player() : Body("player"), _shooting(Spec::get_instance().check("player/projectile/name")), _projectiles(_shooting ? Spec::get_instance().get_str("player/projectile/name") : "", 0), _hp(Spec::get_instance().check("player/hp") ? Spec::get_instance().get_int("player/hp") : 1), _shot(false), _projectile_speed(Spec::get_instance().get_int("player/projectile/speed")), _projectile_offset(Spec::get_instance().get_int("player/projectile/offset/x"), Spec::get_instance().get_int("player/projectile/offset/y")) {
	Input::get_instance().grab("player");
}

Player::~Player() {
	Input::get_instance().release("player");
}

void Player::load() {
	Body::load();

	if (_shooting) {
		_projectiles.clear();
		_projectiles.increase(Spec::get_instance().get_int("player/projectile/initial"));
	}
}

void Player::update(unsigned int ticks, World & world) {
	Body::update(ticks, world);

	if (_shot) {
		Projectile & projectile = _projectiles.create();
		world.add(projectile);

		for (Sprite * observer : get_observers())
			projectile.observe(*observer);

		projectile.set_y(get_y() + get_height()/2 + _projectile_offset[1]);

		int speed = _projectile_speed + std::abs(get_velocity_x());

		if (get_direction() == "left") {
			projectile.set_x(get_x() - projectile.get_width() - _projectile_offset[0]);
			projectile.set_velocity_x(-speed);
		}
		else {
			projectile.set_x(get_x() + get_width() + projectile.get_width() + _projectile_offset[0]);
			projectile.set_velocity_x(speed);
		}

		projectile.reset_origin();

		_shot = false;
	}
}

void Player::shoot() {
	if (_shooting)
		_shot = true;
}

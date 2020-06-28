#ifndef PLAYER_H
#define PLAYER_H

#include "body.h"
#include "pool.h"
#include "projectile.h"
#include "vector2f.h"

class Player : public Body {
	public:
		Player();
		virtual ~Player();

		Player(const Player &) = delete;
		const Player & operator=(const Player &) = delete;

		virtual void load();
		virtual void update(unsigned int ticks, World & world);

		void shoot();

		Pool<Projectile> & get_pool() { return _projectiles; }

		void set_hp(int hp)  { _hp = hp;   }
		int get_hp() const   { return _hp; }

	private:
		bool _shooting;
		Pool<Projectile> _projectiles;

		int _hp;
		bool _shot;

		int _projectile_speed;
		Vector2f _projectile_offset;
};
#endif

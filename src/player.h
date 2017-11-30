#ifndef PLAYER_H
#define PLAYER_H
#include "projectile.h"
#include "pool.h"
#include "sprite.h"

class Player : public Sprite {
	public:
		Player();
		virtual ~Player();

		Player(const Player & player) = delete;
		const Player & operator=(const Player & player) = delete;

		void shoot();

		Pool<Projectile> & get_pool() { return gliders; }

		void set_hp(int val) { hp = val;  }
		int get_hp()   const { return hp; }

	private:
		Pool<Projectile> gliders;

		int hp;
};
#endif

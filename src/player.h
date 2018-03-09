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

		void update(unsigned int ticks, World & world);

		void shoot();

		Pool<Projectile> & get_pool() { return projectiles; }

		void set_hp(int val) { hp = val;  }
		int get_hp()   const { return hp; }

	private:
		bool shooting;
		Pool<Projectile> projectiles;

		int hp;
		bool shot;
};
#endif

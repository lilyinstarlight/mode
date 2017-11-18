#ifndef PLAYER_H
#define PLAYER_H
#include <string>

#include "input.h"
#include "sprite.h"

class Player : public Sprite {
	public:
		Player();
		virtual ~Player();

		Player(const Player & console) = delete;
		const Player & operator=(const Player & console) = delete;

		void set_hp(int val) { hp = val;  }
		int get_hp()   const { return hp; }

		virtual void update(unsigned int ticks);

	private:
		int hp;

		Vector2f movement;
};
#endif

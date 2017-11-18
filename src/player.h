#ifndef PLAYER_H
#define PLAYER_H
#include <string>

#include "input.h"
#include "sprite.h"

class Player : public Sprite {
	public:
		Player();
		Player(const Player & player);

		virtual ~Player();

		const Player & operator=(const Player & player) = delete;

		void set_hp(int val) { hp = val;  }
		int get_hp()   const { return hp; }

		virtual void update(unsigned int ticks);

	private:
		int hp;
};
#endif

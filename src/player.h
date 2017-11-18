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

		virtual void update(unsigned int ticks);

	private:
		int hp;
};
#endif

#ifndef PLAYER_H
#define PLAYER_H
#include <list>
#include <string>

#include "input.h"
#include "observer.h"

class Player : public Sprite {
	public:
		Player(const World & w);
		virtual ~Player() {}

		Player(const Player & console) = delete;
		const Player & operator=(const Player & console) = delete;

		virtual void update(unsigned int ticks);

		void observe(const Observer & observer);
		void ignore(const Observer & observer);

	private:
		std::list<const Observer &> observers;

		unsigned int observer_interval;
		unsigned int observer_timer;
};
#endif

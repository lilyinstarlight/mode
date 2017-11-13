#ifndef OBSERVER_H
#define OBSERVER_H
#include <string>

#include "sprite.h"

class Observer : public Sprite {
	public:
		Observer(const std::string & name, const World & w, bool player = false) : Sprite(name, w, player) {}
		virtual ~Observer() {}

		virtual void signal(const std::string & sig, const Sprite & sprite) {
			script->call(sig, sprite);
		}

	private:
};
#endif

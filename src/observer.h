#ifndef OBSERVER_H
#define OBSERVER_H
#include <string>

#include "sprite.h"

class Observer : public Sprite {
	public:
		virtual void signal(const std::string & sig, const Sprite & sprite) {
			script.call(sig, sprite.name, sprite.position.x, sprite.position.y, sprite.rotation, sprite.velocity.x, sprite.velocity.y, sprite.scale);
		}

	private:
};
#endif

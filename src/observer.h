#ifndef OBSERVER_H
#define OBSERVER_H
#include "sprite.h"

class Observer : public Sprite {
	public:
		virtual void signal(const Sprite & sprite) {
			script.call("signal", sprite.name, sprite.position.x, sprite.position.y, sprite.rotation, sprite.velocity.x, sprite.velocity.y, sprite.scale);
		}

	private:
};
#endif

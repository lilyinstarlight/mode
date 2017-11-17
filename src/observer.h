#ifndef OBSERVER_H
#define OBSERVER_H
#include <string>

#include "sprite.h"

class Observer : public Sprite {
	public:
		Observer(const std::string & name) : Sprite(name) {}
		virtual ~Observer() {}

		virtual void signal(const std::string & sig, const Sprite & sprite) {
			// call script with signal
			get_script().call(sig, sprite);
		}

	private:
};
#endif

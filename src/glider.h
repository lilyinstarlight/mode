#ifndef GLIDER_H
#define GLIDER_H
#include "sprite.h"
#include "vector2f.h"

class Glider : public Sprite {
	public:
		Glider();
		virtual ~Glider() {}

		Glider(const Glider & glider) = delete;
		const Glider & operator=(const Glider & glider) = delete;

		virtual void update(unsigned int ticks);

		bool is_alive() const { return alive; }

		void create();
		void destroy();

	private:
		bool alive;

		Vector2f origin;
		float max;
};
#endif

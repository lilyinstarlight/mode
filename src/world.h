#ifndef WORLD_H
#define WORLD_H
#include "spec.h"

class Viewport;

class World {
	public:
		World() : width(Spec::get_instance().get_int("world/width")), height(Spec::get_instance().get_int("world/height")) {}
		virtual ~World() {}

		World(const World & w) = delete;
		const World & operator=(const World & w) = delete;

		void update(unsigned int) {}
		void draw(const Viewport &) const {}

		int get_width() const  { return width;  }
		int get_height() const { return height; }
	private:
		int width, height;
};
#endif

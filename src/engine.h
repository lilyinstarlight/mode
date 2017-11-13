#ifndef ENGINE_H
#define ENGINE_H
#include <set>

#include <SDL.h>

#include "context.h"
#include "clock.h"
#include "player.h"
#include "viewport.h"
#include "world.h"

class Engine {
	public:
		Engine();
		~Engine();

		Engine(const Engine &) = delete;
		Engine& operator=(const Engine &) = delete;

		void run();

	private:
		World world;
		Viewport viewport;

		Player player;

		Drawable * target;
		std::set<Drawable *, DrawablePointerCompare> drawables;

		void draw() const;
		void update(unsigned int ticks);
};
#endif

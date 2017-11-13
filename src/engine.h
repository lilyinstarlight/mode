#ifndef ENGINE_H
#define ENGINE_H
#include <vector>
#include <SDL.h>

#include "context.h"
#include "clock.h"
#include "player.h"
#include "viewport.h"
#include "world.h"

class Engine {
	public:
		Engine();
		~Engine() {}

		Engine(const Engine &) = delete;
		Engine& operator=(const Engine &) = delete;

		void run();

	private:
		World world;
		Viewport viewport;

		Player player;

		Drawable * target;
		std::vector<Drawable *> drawables;

		void draw() const;
		void update(unsigned int ticks);
};
#endif

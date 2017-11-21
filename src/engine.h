#ifndef ENGINE_H
#define ENGINE_H
#include <SDL.h>

#include "context.h"
#include "clock.h"
#include "player.h"
#include "viewport.h"
#include "world.h"

class Engine {
	public:
		static Engine & get_instance();

		Engine(const Engine &) = delete;
		Engine& operator=(const Engine &) = delete;

		void run();

		const World & get_world()       const { return *world;    }
		World & get_world()                   { return *world;    } // caller can modify world
		const Viewport & get_viewport() const { return *viewport; }
		Viewport & get_viewport()             { return *viewport; } // caller can modify viewport

	private:
		Engine();
		~Engine() {}

		World * world;
		Viewport * viewport;

		void dispatch(const SDL_Event & event);
		void draw() const;
		void update(unsigned int ticks);

};
#endif

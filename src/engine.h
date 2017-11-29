#ifndef ENGINE_H
#define ENGINE_H
#include <string>

#include <SDL.h>

#include "context.h"
#include "clock.h"
#include "hud.h"
#include "player.h"
#include "viewport.h"
#include "world.h"

class Engine {
	public:
		enum State {STOPPED, STOPPING, STARTING, RESTARTING, RUNNING};

		static Engine & get_instance();

		~Engine() {}

		Engine(const Engine &) = delete;
		Engine& operator=(const Engine &) = delete;

		void load(const std::string & w);

		const World & get_world()       const { return *world;    }
		World & get_world()                   { return *world;    } // caller can modify world
		const Viewport & get_viewport() const { return *viewport; }
		Viewport & get_viewport()             { return *viewport; } // caller can modify viewport
		const HUD & get_hud()           const { return *hud;      }
		HUD & get_hud()                       { return *hud;      } // caller can modify hud

		State get_state() const { return state; }

		void start();
		void restart() { state = RESTARTING; }
		void stop() { state = STOPPING; }
		bool is_running() { return state == RUNNING; }

	private:
		Engine();

		void run();

		void dispatch(const SDL_Event & event);
		void draw() const;
		void update(unsigned int ticks);

		std::string loaded;

		World * world;
		Viewport * viewport;
		HUD * hud;

		State state;
};
#endif

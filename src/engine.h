#ifndef ENGINE_H
#define ENGINE_H
#include <string>

#include <SDL.h>

#include "console.h"
#include "context.h"
#include "clock.h"
#include "editor.h"
#include "hud.h"
#include "player.h"
#include "viewport.h"
#include "world.h"

class Engine {
	public:
		enum class State { STOPPED, STOPPING, STARTING, RESTARTING, RUNNING };

		static Engine & get_instance();

		~Engine() {}

		Engine(const Engine &) = delete;
		const Engine & operator=(const Engine &) = delete;

		void load(const std::string & world);

		const World & get_world() const       { return *_world;    }
		World & get_world()                   { return *_world;    } // caller can modify world
		const Viewport & get_viewport() const { return *_viewport; }
		Viewport & get_viewport()             { return *_viewport; } // caller can modify viewport
		const HUD & get_hud() const           { return *_hud;      }
		HUD & get_hud()                       { return *_hud;      } // caller can modify hud
		const Console & get_console() const   { return *_console;  }
		Console & get_console()               { return *_console;  } // caller can modify console
		const Editor & get_editor() const     { return *_editor;   }
		Editor & get_editor()                 { return *_editor;   } // caller can modify editor

		State get_state() const { return _state; }

		void start();
		void restart() { _state = State::RESTARTING; }
		void stop() { _state = State::STOPPING; }
		bool is_running() { return _state == State::RUNNING; }

	private:
		Engine();

		void run();

		void dispatch(const SDL_Event & event);
		void draw() const;
		void update(unsigned int ticks);

		std::string _loaded;

		World * _world;
		Viewport * _viewport;
		HUD * _hud;
		Console * _console;
		Editor * _editor;

		State _state;
};
#endif

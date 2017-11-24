#include "console.h"
#include "spec.h"
#include "text.h"

#include "engine.h"

Engine & Engine::get_instance() {
	static Engine engine;
	return engine;
}

Engine::Engine() : world(nullptr), viewport(nullptr), hud(nullptr), state(STOPPED) {}

void Engine::start() {
	state = STARTING;

	for(;;) {
		run();

		switch (state) {
			case RESTARTING:
				continue;

			case STOPPING:
				state = STOPPED;
				return;

			default:
				return;
		}
	}
}

void Engine::run() {
	hud = new HUD();
	world = new World();
	viewport = new Viewport(*world);

	world->init();

	viewport->track(&world->get_player());

	SDL_Event event;

	Uint32 last = Clock::get_instance().get_ticks();
	Uint32 ticks = 0;

	state = RUNNING;
	while (state == RUNNING) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				// handle closing window
				state = STOPPING;
				break;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					// handle pressing escape
					state = STOPPING;
					break;
				}

				if (event.key.keysym.sym == SDLK_p) {
					// (un)pause clock
					if (Clock::get_instance().is_paused())
						Clock::get_instance().start();
					else
						Clock::get_instance().pause();
				}
			}

			dispatch(event);
		}

		// increment and draw frames if time passed
		ticks = Clock::get_instance().get_ticks();

		if (ticks - last > 0)
			Clock::get_instance().incr_frame();

		draw();
		update(ticks - last);

		last = ticks;
	}

	delete viewport;
	delete world;

	delete hud;
}

void Engine::dispatch(const SDL_Event & event) {
	// update world
	world->dispatch(event);

	// update hud and viewport
	hud->dispatch(event);
	viewport->dispatch(event);

	// update console
	Console::get_instance().dispatch(event);
}

void Engine::draw() const {
	// draw world
	world->draw(*viewport);

	// draw console and hud on top of world

	// draw hud and viewport
	hud->draw(*viewport);
	viewport->draw();

	// draw console on top
	Console::get_instance().draw(*viewport);

	// swap renderer buffers
	SDL_RenderPresent(Context::get_instance().get_renderer());
}

void Engine::update(unsigned int ticks) {
	// update world
	world->update(ticks);

	// update hud and viewport
	hud->update(ticks);
	viewport->update(ticks);

	// update console
	Console::get_instance().update(ticks);
}

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
	Clock::get_instance().start();

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
				else if (event.key.keysym.sym == SDLK_TAB) {
					// (un)pause clock
					if (Clock::get_instance().is_running())
						Clock::get_instance().pause();
					else
						Clock::get_instance().start();
				}
				else if (event.key.keysym.sym == SDLK_F1) {
					// toggle hud
					hud->toggle();
				}
			}

			dispatch(event);
		}

		// increment and draw frames if time passed
		Clock::get_instance().incr_frame();
		draw();

		ticks = Clock::get_instance().get_ticks();
		update(ticks - last);
		last = ticks;
	}

	delete viewport;
	delete world;

	delete hud;

	Clock::get_instance().stop();
}

void Engine::dispatch(const SDL_Event & event) {
	// update world
	if (Clock::get_instance().is_running())
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
	if (Clock::get_instance().is_running())
		world->update(ticks);

	// update hud and viewport
	hud->update(ticks);
	viewport->update(ticks);

	// update console
	Console::get_instance().update(ticks);
}

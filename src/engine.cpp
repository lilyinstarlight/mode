#include <iostream>

#include "console.h"
#include "hud.h"
#include "spec.h"
#include "text.h"

#include "engine.h"

Engine & Engine::get_instance() {
	static Engine engine;
	return engine;
}

Engine::Engine() : world(nullptr), viewport(nullptr) {}

void Engine::run() {
	world = new World();

	world->init();

	viewport = new Viewport(*world);

	SDL_Event event;

	Uint32 last = Clock::get_instance().get_ticks();
	Uint32 ticks = 0;

	bool running = true;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				// handle closing window
				running = false;
				break;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q) {
					// handle pressing q or escape
					running = false;
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
		if (ticks - last > 0) {
			Clock::get_instance().incr_frame();

			std::cout << Clock::get_instance().get_fps() << " FPS        \r";

			draw();
			update(ticks - last);

			last = ticks;
		}
	}

	delete viewport;
	delete world;
}

void Engine::dispatch(const SDL_Event & event) {
	// update world
	world->dispatch(event);

	// update console and hud
	Console::get_instance().dispatch(event);
	HUD::get_instance().dispatch(event);

	// update viewport
	viewport->dispatch(event);
}

void Engine::draw() const {
	// draw world
	world->draw(*viewport);

	// draw console and hud on top of world
	Console::get_instance().draw(*viewport);
	HUD::get_instance().draw(*viewport);

	// draw viewport
	viewport->draw();

	// swap renderer buffers
	SDL_RenderPresent(Context::get_instance().get_renderer());
}

void Engine::update(unsigned int ticks) {
	// update world
	world->update(ticks);

	// update console and hud
	Console::get_instance().update(ticks);
	HUD::get_instance().update(ticks);

	// update viewport
	viewport->update(ticks);
}

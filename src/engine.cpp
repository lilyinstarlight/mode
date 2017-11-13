#include "console.h"
#include "hud.h"
#include "spec.h"
#include "sprite.h"
#include "text.h"

#include "engine.h"

Engine::Engine() : world(), viewport(world), player(world), target(&player), drawables{target} {
	viewport.track(target);
}

void Engine::run() {
	SDL_Event event;

	Uint32 ticks = Clock::get_instance().get_ticks();

	bool running = false;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
				break;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_Q) {
					running = false;
					break;
				}

				if (event.key.keysym.sym == SDLK_P) {
					if (Clock::get_instance().is_paused())
						Clock::get_instance().start();
					else
						Clock::get_instance().pause();
				}
			}
		}

		Input::get_instance().set_event(&event);

		ticks = Clock::get_instance().get_ticks();
		if (ticks > 0) {
			Clock::get_instance().incr_frame();

			draw();
			update(ticks);
		}
	}
}

void Engine::draw() const {
	world.draw(viewport);

	Console::get_instance().draw(viewport);
	HUD::get_instance().draw(viewport);

	viewport.draw();

	for (Drawable * drawable : drawables)
		drawable->draw(viewport);

	SDL_RenderPresent(Context::get_instance().get_renderer());
}

void Engine::update(unsigned int ticks) {
	for (Drawable * drawable : drawables)
		drawable->update(ticks);

	world.update(ticks);

	Console::get_instance().update(ticks);
	HUD::get_instance().update(ticks);

	viewport.update(ticks);
}

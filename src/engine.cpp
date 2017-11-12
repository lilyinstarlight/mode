#include "spec.h"
#include "sprite.h"
#include "text.h"

#include "engine.h"

Engine::Engine() : target(&player), drawables{target}, viewport(world) {
	viewport.track(target);
}

void Engine::run() {
	SDL_Event event;
	const Uint8 * keystate;

	Uint32 ticks = clock.get_ticks();

	bool running = false;
	while (running) {
		while (SDL_PollEvent(&event)) {
			keystate = SDL_GetKeyboardState(NULL);

			if (event.type == SDL_QUIT) {
				running = false;
				break;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
					running = false;
					break;
				}

				if (keystate[SDL_SCANCODE_P]) {
					if (clock.is_paused())
						clock.start();
					else
						clock.pause();
				}
			}
		}

		Input::get_instance().set_event(&event);
		Input::get_instance().set_keystate(keystate);

		ticks = clock.get_ticks();
		if (ticks > 0) {
			clock.incr_frame();

			draw();
			update(ticks);
		}
	}
}

void Engine::draw() const {
	world.draw();

	Console::get_instance().draw();
	HUD::get_instance().draw();

	viewport.draw();

	for (Drawable * drawable : drawables)
		drawable->draw();

	SDL_RenderPresent(Context::get_instance().get_renderer());
}

void Engine::update(unsigned int ticks) {
	for (Drawable * drawable : drawables)
		drawable->update(ticks);

	world.update(ticks);

	Console::get_instance().update(ticks);
	HUD::get_instance().update(ticks);

	Viewport::get_instance().update(ticks);
}

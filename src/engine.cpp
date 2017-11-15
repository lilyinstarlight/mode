#include "background.h"
#include "console.h"
#include "hud.h"
#include "spec.h"
#include "sprite.h"
#include "text.h"

#include "engine.h"

Engine::Engine() : world(), viewport(world), player(world), target(&player), drawables{&player} {
	viewport.track(target);

	// get top level elements and add applicable ones to drawables
	for (const std::string & str : Spec::get_instance().get_tops()) {
		if (Spec::get_instance().check(str + "/type")) {
			if (Spec::get_instance().get_str(str + "/type") == "background") {
				drawables.insert(new Background(str, world));
			}
			else if (Spec::get_instance().get_str(str + "/type") == "sprite") {
				drawables.insert(new Sprite(str, world));
			}
		}
	}

}

Engine::~Engine() {
	// free added drawables
	for (Drawable * drawable : drawables) {
		if (drawable != &player)
			free(drawable);
	}
}

void Engine::run() {
	SDL_Event event;
	const Uint8 * keystate;

	Uint32 ticks = Clock::get_instance().get_ticks();

	bool running = false;
	while (running) {
		while (SDL_PollEvent(&event)) {
			keystate = SDL_GetKeyboardState(NULL);

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
		}

		// record input event
		Input::get_instance().set_event(&event);
		Input::get_instance().set_keystate(keystate);

		// increment and draw frames if time passed
		ticks = Clock::get_instance().get_ticks();
		if (ticks > 0) {
			Clock::get_instance().incr_frame();

			draw();
			update(ticks);
		}
	}
}

void Engine::draw() const {
	// draw world
	world.draw(viewport);

	// draw ordered drawables
	for (Drawable * drawable : drawables)
		drawable->draw(viewport);

	// draw console and hud on top of world
	Console::get_instance().draw(viewport);
	HUD::get_instance().draw(viewport);

	// draw viewport
	viewport.draw();

	// swap renderer buffers
	SDL_RenderPresent(Context::get_instance().get_renderer());
}

void Engine::update(unsigned int ticks) {
	// update world
	world.update(ticks);

	// update drawables
	for (Drawable * drawable : drawables)
		drawable->update(ticks);

	// update console and hud
	Console::get_instance().update(ticks);
	HUD::get_instance().update(ticks);

	// update viewport
	viewport.update(ticks);
}

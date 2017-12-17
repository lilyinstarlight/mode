#include "sound.h"
#include "spec.h"
#include "text.h"

#include "engine.h"

Engine & Engine::get_instance() {
	static Engine engine;
	return engine;
}

Engine::Engine() : loaded("game"), world(nullptr), viewport(nullptr), hud(nullptr), console(nullptr), editor(nullptr), state(STOPPED) {}

void Engine::load(const std::string & w) {
	loaded = w;

	if (state == RUNNING)
		state = RESTARTING;
}

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
	Spec::get_instance().load(loaded);
	Context::get_instance().reload();
	Sound::get_instance().reload();

	if (Spec::get_instance().check("editor/box/r"))
		editor = new Editor();
	if (Spec::get_instance().check("console/box/r"))
		console = new Console();

	if (Spec::get_instance().check("hud/type"))
		hud = new HUD();

	world = new World();
	viewport = new Viewport();

	world->init();
	viewport->track(&world->get_player());

	Clock::get_instance().start();

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
			else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
				if (editor && editor->is_open()) {
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						editor->close();
					}
				}
				else {
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						if (console && console->is_open()) {
							console->close();
						}
						else {
							// handle pressing escape
							state = STOPPING;
							break;
						}
					}
					else if (event.key.keysym.sym == SDLK_TAB) {
						// (un)pause clock
						if (Clock::get_instance().is_running()) {
							Clock::get_instance().pause();
							if (hud)
								hud->open();
						}
						else {
							Clock::get_instance().start();
							if (hud)
								hud->close();
						}
					}
					else if (event.key.keysym.sym == SDLK_F1) {
						// toggle hud
						if (hud)
							hud->toggle();
					}
					else if (event.key.keysym.sym == SDLK_BACKQUOTE) {
						// toggle console
						if (console)
							console->toggle();
					}
					else if (event.key.keysym.sym == SDLK_BACKSPACE) {
						if (!(console && console->is_open())) {
							// restart level
							restart();
						}
					}
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

	Clock::get_instance().stop();

	delete viewport;
	delete world;

	if (hud)
		delete hud;

	if (console)
		delete console;
	if (editor)
		delete editor;
}

void Engine::dispatch(const SDL_Event & event) {
	// update world
	if (Clock::get_instance().is_running())
		world->dispatch(event);

	// update hud and stuff
	if (hud)
		hud->dispatch(event);
	viewport->dispatch(event);
	if (editor)
		editor->dispatch(event);
	if (console)
		console->dispatch(event);
}

void Engine::draw() const {
	// draw world
	world->draw(*viewport);

	// draw hud and stuff
	if (hud)
		hud->draw(*viewport);
	viewport->draw();
	if (editor)
		editor->draw(*viewport);
	if (console)
		console->draw(*viewport);

	// swap renderer buffers
	SDL_RenderPresent(Context::get_instance().get_renderer());
}

void Engine::update(unsigned int ticks) {
	// update world
	if (Clock::get_instance().is_running())
		world->update(ticks);

	// update hud and viewport
	if (hud)
		hud->update(ticks, *world);
	viewport->update(ticks, *world);
	if (editor)
		editor->update(ticks, *world);
	if (console)
		console->update(ticks, *world);
}

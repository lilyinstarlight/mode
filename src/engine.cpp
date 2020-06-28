#include "input.h"
#include "sound.h"
#include "spec.h"
#include "text.h"

#include "engine.h"

Engine & Engine::get_instance() {
	static Engine engine;
	return engine;
}

Engine::Engine() : _loaded("game"), _world(nullptr), _viewport(nullptr), _hud(nullptr), _console(nullptr), _editor(nullptr), _state(State::STOPPED) {}

void Engine::load(const std::string & world) {
	_loaded = world;

	if (_state == State::RUNNING)
		_state = State::RESTARTING;
}

void Engine::start() {
	_state = State::STARTING;

	for (;;) {
		run();

		switch (_state) {
			case State::RESTARTING:
				continue;

			case State::STOPPING:
				_state = State::STOPPED;
				return;

			default:
				return;
		}
	}
}

void Engine::run() {
	Spec::get_instance().load(_loaded);
	Context::get_instance().reload();
	Sound::get_instance().reload();

	if (Spec::get_instance().check("editor/box/r"))
		_editor = new Editor();
	if (Spec::get_instance().check("console/box/r"))
		_console = new Console();

	if (Spec::get_instance().check("hud/type"))
		_hud = new HUD();

	_world = new World();
	_viewport = new Viewport();

	_world->init();
	_viewport->track(&_world->get_player());

	Clock::get_instance().start();

	SDL_Event event;

	Uint32 last = Clock::get_instance().get_ticks();
	Uint32 ticks = 0;

	_state = State::RUNNING;
	while (_state == State::RUNNING) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				// handle closing window
				_state = State::STOPPING;
				break;
			}
			else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
				if (_editor && _editor->is_open()) {
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						_editor->close();
					}
				}
				else {
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						if (_console && _console->is_open()) {
							_console->close();
						}
						else {
							// handle pressing escape
							_state = State::STOPPING;
							break;
						}
					}
					else if (event.key.keysym.sym == SDLK_TAB) {
						// (un)pause clock
						if (Clock::get_instance().is_running()) {
							Clock::get_instance().pause();
							if (_hud)
								_hud->open();
						}
						else {
							Clock::get_instance().start();
							if (_hud)
								_hud->close();
						}
					}
					else if (event.key.keysym.sym == SDLK_F1) {
						// toggle hud
						if (_hud)
							_hud->toggle();
					}
					else if (event.key.keysym.sym == SDLK_BACKQUOTE) {
						// toggle console
						if (_console)
							_console->toggle();
					}
					else if (event.key.keysym.sym == SDLK_BACKSPACE) {
						if (!(_console && _console->is_open())) {
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

	delete _viewport;
	_viewport = nullptr;
	delete _world;
	_world = nullptr;

	if (_hud) {
		delete _hud;
		_hud = nullptr;
	}

	if (_console) {
		delete _console;
		_console = nullptr;
	}

	if (_editor) {
		delete _editor;
		_editor = nullptr;
	}
}

void Engine::dispatch(const SDL_Event & event) {
	// update world
	if (Clock::get_instance().is_running())
		_world->dispatch(event);

	// update hud and stuff
	if (_hud)
		_hud->dispatch(event);
	_viewport->dispatch(event);
	if (_editor)
		_editor->dispatch(event);
	if (_console)
		_console->dispatch(event);

	// update input
	Input::get_instance().dispatch(event);
}

void Engine::draw() const {
	// draw world
	_world->draw(*_viewport);

	// draw hud and stuff
	if (_hud)
		_hud->draw(*_viewport);
	_viewport->draw();
	if (_editor)
		_editor->draw(*_viewport);
	if (_console)
		_console->draw(*_viewport);

	// swap renderer buffers
	SDL_RenderPresent(Context::get_instance().get_renderer());
}

void Engine::update(unsigned int ticks) {
	// update world
	if (Clock::get_instance().is_running())
		_world->update(ticks);

	// update hud and viewport
	if (_hud)
		_hud->update(ticks, *_world);
	_viewport->update(ticks, *_world);
	if (_editor)
		_editor->update(ticks, *_world);
	if (_console)
		_console->update(ticks, *_world);

	// update input
	Input::get_instance().update(ticks);
}

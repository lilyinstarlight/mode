#include <SDL.h>

#include "clock.h"

Clock & Clock::get_instance() {
	static Clock clock;
	return clock;
}

Clock::Clock() : _started(false), _running(false), _start_time(0), _cur_time(0), _frames(0), _frame_time(0), _fps(-1), _fps_gain(0.8) {}

unsigned int Clock::get_ticks() {
	if (_running)
		_cur_time = SDL_GetTicks() - _start_time;

	return _cur_time;
}

int Clock::get_fps() {
	get_ticks();

	if (_cur_time - _frame_time > 1000) {
		int next = _frames*1000/(_cur_time - _frame_time);

		if (_fps < 0) {
			// initialize with straight reading
			_fps = next;
		}
		else {
			// apply low pass filter to subsequent readings
			_fps = _fps_gain*next + (1 - _fps_gain)*_fps;
			_frame_time = _cur_time;
			_frames = 0;
		}
	}

	return _fps;
}

void Clock::incr_frame() {
	if (_running)
		++_frames;
}

void Clock::start() {
	if (_started) {
		if (!_running) {
			// unpause and drift clock
			_running = true;
			_start_time = SDL_GetTicks() - _cur_time;
		}
	}
	else {
		// initialize clock
		_started = true;
		_running = true;
		_start_time = SDL_GetTicks();
		_cur_time = SDL_GetTicks();
		_frames = 0;
		_frame_time = SDL_GetTicks();
		_fps = -1;
	}
}

void Clock::pause() {
	_running = false;
}

void Clock::stop() {
	_started = false;
}

bool Clock::is_running() const {
	return _running;
}

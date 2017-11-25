#include <SDL.h>

#include "clock.h"

Clock & Clock::get_instance() {
	static Clock clock;
	return clock;
}

Clock::Clock() : started(false), running(false), start_time(0), cur_time(0), frames(0), frame_time(0), fps(-1), fps_gain(0.8) {}

unsigned int Clock::get_ticks() {
	if (running)
		cur_time = SDL_GetTicks() - start_time;

	return cur_time;
}

int Clock::get_fps() {
	get_ticks();

	if (cur_time - frame_time > 1000) {
		int next = frames*1000/(cur_time - frame_time);

		if (fps < 0) {
			// initialize with straight reading
			fps = next;
		}
		else {
			// apply low pass filter to subsequent readings
			fps = fps_gain*next + (1 - fps_gain)*fps;
			frame_time = cur_time;
			frames = 0;
		}
	}

	return fps;
}

void Clock::incr_frame() {
	if (running)
		++frames;
}

void Clock::start() {
	if (started) {
		if (!running) {
			// unpause
			running = true;
			start_time += SDL_GetTicks() - cur_time;
		}
	}
	else {
		// initialize clock
		started = true;
		running = true;
		start_time = SDL_GetTicks();
		cur_time = SDL_GetTicks();
		frames = 0;
		frame_time = SDL_GetTicks();
		fps = -1;
	}
}

void Clock::pause() {
	running = false;
}

void Clock::stop() {
	started = false;
}

bool Clock::is_running() const {
	return running;
}

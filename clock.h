#ifndef CLOCK_H
#define CLOCK_H
class Clock {
	public:
		Clock & Clock::get_instance();

		Clock(const Clock & clock) = delete;
		const Clock & operator=(const Clock & clock) = delete;

		unsigned int get_ticks() const;
		int get_fps() const;

		void incr_frame();

		void start();
		void pause();
		void stop();

		bool is_paused() const;

	private:
		Clock();
		~Clock() {}

		bool started, paused;
		unsigned int start_time, cur_time;
		unsigned int frames, frame_time;
		int fps;
		float fps_gain;
};
#endif

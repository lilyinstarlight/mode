#ifndef CLOCK_H
#define CLOCK_H
class Clock {
	public:
		static Clock & get_instance();

		~Clock() {}

		Clock(const Clock & clock) = delete;
		const Clock & operator=(const Clock & clock) = delete;

		unsigned int get_ticks(); // not const since it updates cur_time
		int get_fps(); // not const since it updates fps

		void incr_frame();

		void start();
		void pause();
		void stop();

		bool is_running() const;

	private:
		Clock();

		bool started, running;
		unsigned int start_time, cur_time;
		unsigned int frames, frame_time;
		int fps;
		float fps_gain;
};
#endif

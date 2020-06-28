#ifndef CLOCK_H
#define CLOCK_H
class Clock {
	public:
		static Clock & get_instance();

		~Clock() {}

		Clock(const Clock &) = delete;
		const Clock & operator=(const Clock &) = delete;

		unsigned int get_ticks(); // not const since it updates cur_time
		int get_fps(); // not const since it updates fps

		void incr_frame();

		void start();
		void pause();
		void stop();

		bool is_running() const;

	private:
		Clock();

		bool _started, _running;
		unsigned int _start_time, _cur_time;
		unsigned int _frames, _frame_time;
		int _fps;
		float _fps_gain;
};
#endif

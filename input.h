#ifndef INPUT_H
#define INPUT_H
#include <stack>
#include <string>

class Input {
	public:
		static Input & get_instance() {
			static Input input;
			return input;
		}

		void grab(const std::string & name) {
			list.push_front(name);
		}

		void release(const std::string & name) {
			list.remove(name);
		}

		bool check(const std::string & name) const {
			return list.front == name;
		}

		const SDL_Event * get_event() const  { return event; }
		void set_event(const SDL_Event * ev) { event = ev;   }

		const Uint8 * get_keystate() const  { return keystate; }
		void set_keystate(const Uint8 * ks) { keystate = ks;   }

	private:
		Input();

		const SDL_Event * event;
		const Uint8 * keystate;
};
#endif

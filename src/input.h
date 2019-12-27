#ifndef INPUT_H
#define INPUT_H
#include <list>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

class Input {
	public:
		static Input & get_instance() {
			static Input input;
			return input;
		}

		~Input() {}

		Input(const Input &) = delete;
		const Input & operator=(const Input &) = delete;

		void grab(const std::string & name) {
			// add to front of list
			list.push_front(name);
		}

		void release(const std::string & name) {
			// remove all entries from list
			list.remove(name);
		}

		bool check(const std::string & name) const {
			// check if at front of list
			return list.front() == name;
		}

		const std::string & get() const {
			return list.front();
		}

		std::vector<std::string> get_list() const {
			return std::vector<std::string>(list.begin(), list.end());
		}

		bool get_key(const SDL_Scancode & key) const {
			return static_cast<bool>(SDL_GetKeyboardState(nullptr)[key]);
		}

	private:
		Input() : list{} {}

		std::list<std::string> list;
};
#endif

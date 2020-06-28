#ifndef INPUT_H
#define INPUT_H
#include <list>
#include <string>
#include <vector>

#include <SDL.h>

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
			_list.push_front(name);
		}

		void release(const std::string & name) {
			// remove all entries from list
			_list.remove(name);
		}

		bool check(const std::string & name) const {
			// check if at front of list
			return _list.front() == name;
		}

		const std::string & get() const {
			return _list.front();
		}

		std::vector<std::string> get_list() const {
			return std::vector<std::string>(_list.begin(), _list.end());
		}

		bool get_key(const SDL_Scancode & key) const {
			return static_cast<bool>(SDL_GetKeyboardState(nullptr)[key]);
		}

	private:
		Input() : _list{} {}

		std::list<std::string> _list;
};
#endif

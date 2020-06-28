#ifndef INPUT_H
#define INPUT_H
#include <list>
#include <string>
#include <unordered_set>
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

		void dispatch(const SDL_Event & event) {
			if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
				_pressed.insert(event.key.keysym.scancode);
		}

		void update(unsigned int) {
			_pressed.clear();
		}

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

		std::vector<SDL_Scancode> get_pressed() const {
			return std::vector<SDL_Scancode>(_pressed.begin(), _pressed.end());
		}

		bool get_key(const SDL_Scancode & key) const {
			return static_cast<bool>(SDL_GetKeyboardState(nullptr)[key]);
		}

		bool check_key(const SDL_Scancode & key) const {
			return _pressed.find(key) != _pressed.end();
		}

	private:
		Input() : _list{}, _pressed{} {}

		std::list<std::string> _list;

		std::unordered_set<SDL_Scancode> _pressed;
};
#endif

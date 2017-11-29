#ifndef POOL_H
#define POOL_H
#include <string>
#include <vector>

#include "sprite.h"

template<typename T, typename... Args>
class Pool {
	public:
		Pool(Args...) : initial(5), name(n) {
			for (int i = 0; i < initial; ++i)
				free.push_back(new T(Args...));
		}

		Pool(const Pool & p) : used{}, free{} {
			for (T * obj : p.used)
				used.push_back(new T(*obj));

			for (T * obj : free)
				free.push_back(new T(*obj));
		}

		~Pool() {
			for (T * obj : used)
				delete obj;

			for (T * obj : free)
				delete obj;
		}

		const Pool & operator=(const Pool &) = delete;

		virtual void dispatch(const SDL_Event & event) {
			for (T * obj : used) {
				if (obj->is_alive())
					obj->dispatch();
				else
					destroy(*obj);
			}
		}

		virtual void draw(const Viewport & viewport) const {
			for (T * obj : used) {
				if (obj->is_alive())
					obj->draw(viewport);
				else
					destroy(*obj);
			}
		}

		virtual void update(unsigned int ticks) {
			for (T * obj : used) {
				if (obj->is_alive())
					obj->update(ticks);
				else
					destroy(*obj);
			}
		}

		const T & create() {
			if (free.empty()) {
				used.push_back(new T(Args...));
			}
			else {
				used.push_back(free.back());
				free.pop_back();
			}

			used.back()->create();

			return *used.back();
		}

		void destroy(const T & obj) {
			// find object
			std::vector<T *>::iterator it;
			for (it = used.begin(); it != used.end(); ++it) {
				if (it == &obj) {
					free.push_back(*it);
					used.remove(it);

					free.back()->destroy();

					return;
				}
			}

			throw std::runtime_error("Tried to destroy non-existent pool object " + name);
		}

	private:
		int initial;
		std::string name;

		std::vector<T *> used;
		std::vector<T *> free;
};
#endif

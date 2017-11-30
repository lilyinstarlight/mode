#ifndef POOL_H
#define POOL_H
#include <string>
#include <vector>

#include "sprite.h"

template<typename T>
class Pool {
	public:
		Pool(const std::string & n) : initial(5), name(n), used{}, free{} {
			for (int i = 0; i < initial; ++i)
				free.push_back(new T(name));
		}

		Pool(const Pool & p) : used{}, free{} {
			for (T * obj : p.used)
				used.push_back(new T(*obj));

			for (T * obj : free)
				free.push_back(new T(*obj));
		}

		virtual ~Pool() {
			for (T * obj : used)
				delete obj;

			for (T * obj : free)
				delete obj;
		}

		const Pool & operator=(const Pool &) = delete;

		virtual void dispatch(const SDL_Event & event) {
			for (T * obj : used) {
				if (obj->is_alive())
					obj->dispatch(event);
				else
					destroy(*obj);
			}
		}

		virtual void draw(const Viewport & viewport) const {
			for (T * obj : used)
				obj->draw(viewport);
		}

		virtual void update(unsigned int ticks) {
			for (T * obj : used) {
				if (obj->is_alive())
					obj->update(ticks);
				else
					destroy(*obj);
			}
		}

		T & create() {
			if (free.empty()) {
				used.push_back(new T(name));
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
			typename std::vector<T *>::iterator it;
			for (it = used.begin(); it != used.end(); ++it) {
				if (*it == &obj) {
					free.push_back(*it);
					it = used.erase(it);

					free.back()->destroy();

					return;
				}
			}

			throw std::runtime_error("Tried to destroy non-existent pool object in " + name);
		}

	private:
		int initial;
		std::string name;

		std::vector<T *> used;
		std::vector<T *> free;
};
#endif

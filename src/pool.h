#ifndef POOL_H
#define POOL_H
#include <string>
#include <vector>

#include "sprite.h"

template<typename T>
class Pool {
	public:
		Pool(const std::string & n, int initial) : name(n), used{}, free{} {
			for (int i = 0; i < initial; ++i) {
				T * t = new T(name, *this);
				t->load();

				free.push_back(t);
			}
		}

		Pool(const Pool & p) : used{}, free{} {
			for (T * obj : p.used) {
				T * t = new T(*obj);
				t->load();

				used.push_back(t);
			}

			for (T * obj : free) {
				T * t = new T(*obj);
				t->load();

				free.push_back(t);
			}
		}

		virtual ~Pool() {
			for (T * obj : used)
				delete obj;

			for (T * obj : free)
				delete obj;
		}

		const Pool & operator=(const Pool &) = delete;

		const std::vector<T *> & get_used() const { return used; }

		const std::vector<T *> & get_free() const { return free; }

		T & create() {
			if (free.empty()) {
				T * t = new T(name, *this);
				t->load();
				used.push_back(t);
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
		std::string name;

		std::vector<T *> used;
		std::vector<T *> free;
};
#endif

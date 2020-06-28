#ifndef POOL_H
#define POOL_H
#include <stdexcept>
#include <string>
#include <vector>

#include "sprite.h"
#include "util.h"

template <typename T>
class Pool {
	public:
		Pool(const std::string & n, unsigned int initial = 0) : _name(n), _used{}, _free{} {
			increase(initial);
		}

		Pool(const Pool & p) : _name(p.name), _used{}, _free{} {
			for (T * obj : p.used) {
				T * t = new T(*obj);
				t->load();

				_used.push_back(t);
			}

			for (T * obj : _free) {
				T * t = new T(*obj);
				t->load();

				_free.push_back(t);
			}
		}

		~Pool() {
			for (T * obj : _used)
				delete obj;

			for (T * obj : _free)
				delete obj;
		}

		const Pool & operator=(const Pool &) = delete;

		const std::vector<T *> & get_used() const { return _used; }
		const std::vector<T *> & get_free() const { return _free; }

		void increase(unsigned int count) {
			for (unsigned int idx = 0; idx < count; ++idx) {
				T * t = new T(_name, *this);
				t->load();

				_free.push_back(t);
			}
		}

		void decrease(unsigned int count) {
			count = Util::min(count, _free.size());

			for (unsigned int idx = 0; idx < count; ++idx) {
				typename std::vector<T *>::iterator it = _free.begin();
				delete *it;
				_free.erase(it);
			}
		}

		void clear() {
			for (T * obj : _free)
				delete obj;

			_free.clear();
		}

		T & create() {
			if (_free.empty()) {
				T * t = new T(_name, *this);
				t->load();

				_used.push_back(t);
			}
			else {
				_used.push_back(_free.back());
				_free.pop_back();
			}

			_used.back()->revive();

			return *_used.back();
		}

		void destroy(const T & obj) {
			// find object
			typename std::vector<T *>::iterator it;
			for (it = _used.begin(); it != _used.end(); ++it) {
				if (*it == &obj) {
					_free.push_back(*it);
					it = _used.erase(it);

					_free.back()->kill();

					return;
				}
			}

			throw std::runtime_error("Tried to destroy non-existent pool object in " + _name);
		}

	private:
		std::string _name;

		std::vector<T *> _used;
		std::vector<T *> _free;
};
#endif

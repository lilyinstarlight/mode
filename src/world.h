#ifndef WORLD_H
#define WORLD_H
#include <set>
#include <unordered_set>
#include <utility>

#include "drawable.h"
#include "player.h"
#include "vector2f.h"

class Viewport;

class World {
	public:
		World();
		~World();

		World(const World &) = delete;
		const World & operator=(const World &) = delete;

		void init();

		template <typename T, typename... Args>
		T * create(Args... args) {
			T * t = new T(args...);
			t->load();
			_owning.insert(t);
			return t;
		}

		void add(Drawable & drawable);
		void remove(Drawable & drawable);
		bool check(const Drawable * drawable) const;
		Drawable * get(const std::string & name) const;
		void destroy(Drawable * drawable);

		void dispatch(const SDL_Event & event);
		void update(unsigned int ticks);
		void draw(const Viewport & viewport) const;

		template <typename T>
		std::vector<T *> get_all() const {
			std::vector<T *> all;

			for (Drawable * drawable : _drawables) {
				// try to cast the object
				T * obj = dynamic_cast<T *>(drawable);
				if (obj)
					all.push_back(obj);
			}

			return all;
		}

		template <typename T>
		std::pair<T *, Vector2f> cast(Vector2f point, float direction, std::string type = "") const {
			T * close = nullptr;
			Vector2f hit(point);
			float mag = _far;

			// calculate the ray to cast
			Vector2f ray(std::cos(direction), -std::sin(direction));

			for (Drawable * drawable : _drawables) {
				// try to cast the object
				T * obj = dynamic_cast<T *>(drawable);
				if (obj && (type.empty() || obj->get_type() == type)) {
					// calculate four corners
					std::vector<Vector2f> corners{Vector2f(obj->get_x(), obj->get_y()), Vector2f(obj->get_x() + obj->get_width(), obj->get_y()), Vector2f(obj->get_x() + obj->get_width(), obj->get_y() + obj->get_height()), Vector2f(obj->get_x(), obj->get_y() + obj->get_height())};

					for (unsigned int corner = 0; corner < corners.size(); ++corner) {
						// calculate line segment from corners
						Vector2f & cur = corners[corner];
						Vector2f & next = corners[(corner + 1) % corners.size()];

						Vector2f cur_ray(next[0] - cur[0], next[1] - cur[1]);

						// calculate intersection point
						Vector2f delta(cur[0] - point[0], cur[1] - point[1]);
						float det = cur_ray[0] * ray[1] - cur_ray[1] * ray[0];

						if (det == 0)
							continue;

						float u = (delta[1] * cur_ray[0] - delta[0] * cur_ray[1]) / det;
						float v = (delta[1] * ray[0] - delta[0] * ray[1]) / det;

						// if it intersects the positive end of the ray and inside of the line segment
						if (u > 0.0 && v > 0.0 && v < 1.0) {
							Vector2f pt = point + ray*u;
							float m = (pt - point).magnitude();

							if (m < mag) {
								close = obj;
								hit = pt;
								mag = m;
							}
						}
					}
				}
			}

			return std::make_pair(close, hit);
		}

		const Player & get_player() const { return *_player; }
		Player & get_player()             { return *_player; } // caller can modify player

		int get_width() const  { return _width;  }
		int get_height() const { return _height; }

		float get_far() const { return _far; }

	private:
		int _width, _height;

		float _far;

		Player * _player;
		std::unordered_set<Drawable *> _owning;
		std::set<Drawable *, DrawablePointerCompare> _drawables; // compare pointers to keep drawables unique and ordered
		std::unordered_set<Drawable *> _destroyables;
		std::unordered_set<Drawable *> _removables;
};
#endif

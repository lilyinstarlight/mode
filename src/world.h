#ifndef WORLD_H
#define WORLD_H
#include <set>
#include <unordered_set>
#include <utility>

#include "collision.h"
#include "drawable.h"
#include "player.h"

class Viewport;

class World {
	public:
		World();
		~World();

		World(const World & w) = delete;
		const World & operator=(const World & w) = delete;

		void init();

		template<typename T, typename... Args>
		T * create(Args... args) {
			T * t = new T(args...);
			t->load();
			owning.insert(t);
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

		std::pair<Drawable *, Vector2f> cast(Vector2f point, float direction);

		const Player & get_player() const { return *player; }
		Player & get_player()             { return *player; } // caller can modify player

		int get_width() const  { return width;  }
		int get_height() const { return height; }
	private:
		int width, height;

		float far;

		Player * player;
		std::unordered_set<Drawable *> owning;
		std::set<Drawable *, DrawablePointerCompare> drawables; // compare points to keep drawables unique and ordered
		std::unordered_set<Drawable *> destroyables;
		std::unordered_set<Drawable *> removables;
};
#endif

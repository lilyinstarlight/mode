#ifndef WORLD_H
#define WORLD_H
#include <set>
#include <unordered_set>

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

		void add(Drawable & drawable);
		void remove(Drawable & drawable);
		Drawable * get(const std::string & name);

		void dispatch(const SDL_Event & event);
		void update(unsigned int ticks);
		void draw(const Viewport & viewport) const;

		const Player & get_player() const { return *player; }
		Player & get_player()             { return *player; } // caller can modify player

		int get_width() const  { return width;  }
		int get_height() const { return height; }
	private:
		int width, height;

		Player * player;
		std::unordered_set<Drawable *> owning;
		std::set<Drawable *, DrawablePointerCompare> drawables; // compare points to keep drawables unique and ordered
};
#endif

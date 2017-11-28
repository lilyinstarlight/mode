#ifndef SPRITE_H
#define SPRITE_H
#include <unordered_map>
#include <list>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "collision.h"
#include "drawable.h"
#include "script.h"
#include "sheet.h"

class Sprite : public Drawable {
	public:
		Sprite(const std::string & name);
		Sprite(const Sprite & s);

		virtual ~Sprite();

		Sprite() = delete;
		Sprite & operator=(const Sprite & s) = delete;

		virtual void dispatch(const SDL_Event & event);
		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int ticks);

		virtual const Image * get_image() const;

		virtual int get_width() const;
		virtual int get_height() const;

		virtual const SDL_Surface * get_surface() const;

		std::string get_state() const;
		void set_state(const std::string & s);
		void push_state(const std::string & s);

		void observe(Sprite & observer);
		void ignore(Sprite & observer);

		void signal(const std::string & sig, Sprite & sprite);

		void inject();

	private:
		Script * script;

		NoneCollisionStrategy none_strategy;
		RectangularCollisionStrategy rectangular_strategy;
		CircularCollisionStrategy circular_strategy;
		PixelCollisionStrategy pixel_strategy;

		CollisionStrategy * collision_strategy;
		std::list<Sprite *> observers;

		std::unordered_map<std::string, Sheet *> sheets;
		std::stack<std::pair<std::string, unsigned int>> state;

		unsigned int observer_interval;

		float frame_timer;
		float observer_timer;
};
#endif

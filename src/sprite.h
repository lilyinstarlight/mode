#ifndef SPRITE_H
#define SPRITE_H
#include <map>
#include <list>
#include <string>
#include <vector>

#include "collision.h"
#include "drawable.h"
#include "script.h"
#include "sheet.h"

class Observer;

class Sprite : public Drawable {
	public:
		Sprite(const std::string & name);
		Sprite(const Sprite & s);

		Sprite() = delete;
		Sprite & operator=(const Sprite & s) = delete;

		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int ticks);

		void observe(Observer & observer);
		void ignore(Observer & observer);

		virtual const Image * get_image() const {
			return sheets.at(state)->get_image(frame);
		}

		int get_width() const {
			return get_scale()*sheets.at(state)->get_image(frame)->get_width();
		}

		int get_height() const {
			return get_scale()*sheets.at(state)->get_image(frame)->get_height();
		}

		virtual const SDL_Surface * get_surface() const {
			return sheets.at(state)->get_image(frame)->get_surface();
		}

		std::string get_state() const         { return state; }
		void set_state(const std::string & s) { state = s;    }

		void inject(const Player & player);

	protected:
		Script * script;

		NoneCollisionStrategy none_strategy;
		RectangularCollisionStrategy rectangular_strategy;
		CircularCollisionStrategy circular_strategy;
		PixelCollisionStrategy pixel_strategy;

		CollisionStrategy * collision_strategy;
		std::list<Observer *> observers;

	private:
		std::map<std::string, Sheet *> sheets;
		std::string state;

		unsigned int frame;

		unsigned int script_interval;
		unsigned int observer_interval;

		float frame_timer;
		float script_timer;
		float observer_timer;

	friend Script;
};
#endif

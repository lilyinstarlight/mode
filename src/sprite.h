#ifndef SPRITE_H
#define SPRITE_H
#include <string>
#include <list>
#include <vector>

#include "collision.h"
#include "drawable.h"
#include "script.h"
#include "world.h"

class Observer;

class Sprite : public Drawable {
	public:
		Sprite(const std::string & name, const World & w, bool player = false);
		Sprite(const Sprite & s);

		Sprite() = delete;
		Sprite & operator=(const Sprite & s) = delete;

		virtual void draw() const;
		virtual void update(unsigned int ticks);

		void observe(const Observer & observer);
		void ignore(const Observer & observer);

		virtual const Image * get_image() const {
			return sheet[frame];
		}

		int get_width() const {
			return get_scale()*sheet[frame]->get_width();
		}

		int get_height() const {
			return get_scale()*sheet[frame]->get_height();
		}

		virtual const SDL_Surface * get_surface() const {
			return sheet[frame]->get_surface();
		}

	protected:
		const World & world;

		Script * script;

		RectangularCollisionStrategy rectangular_strategy;
		CircularCollisionStrategy circular_strategy;
		PixelCollisionStrategy pixel_strategy;

		CollisionStrategy * collision_strategy;
		std::list<const Observer *> observers;

	private:
		std::vector<Image *> sheet;

		unsigned int frame;
		unsigned int frames;
		unsigned int interval;

		unsigned int script_interval;
		unsigned int observer_interval;

		float frame_timer;
		float script_timer;
		float observer_timer;
};
#endif

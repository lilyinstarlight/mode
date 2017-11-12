#ifndef SPRITE_H
#define SPRITE_H
#include <string>
#include <vector>

#include "drawable.h"
#include "collision.h"
#include "world.h"

class Sprite : public Drawable {
	public:
		Sprite(const std::string & name, const World & w, bool player = false);
		Sprite(const Sprite & s);

		Sprite & operator=(const Sprite & s) = delete;

		virtual void draw() const;
		virtual void update(unsigned int ticks);

		virtual const Image * get_image() const {
			return sheet[currentFrame];
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

	private:
		const World & world;

		Script * script;
		std::vector<Image *> sheet;

		RectangularCollisionStrategy rectangular_strategy;
		CircularCollisionStrategy circular_strategy;
		PixelCollisionStrategy pixel_strategy;

		CollisionStrategy * collision_strategy;

		unsigned int frame;
		unsigned int frames;
		unsigned int interval;

		unsigned int script_interval;

		float frame_timer;
		float script_timer;
};
#endif

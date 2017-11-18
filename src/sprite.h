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

class Sprite : public Drawable {
	public:
		Sprite(const std::string & name);
		Sprite(const Sprite & s);

		virtual ~Sprite();

		Sprite() = delete;
		Sprite & operator=(const Sprite & s) = delete;

		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int ticks);

		virtual const Image * get_image() const;

		virtual int get_width() const;
		virtual int get_height() const;

		virtual const SDL_Surface * get_surface() const;

		const Script & get_script() const { return *script; }
		Script & get_script()             { return *script; } // caller can modify script

		std::string get_state() const         { return state; }
		void set_state(const std::string & s) { state = s;    }

		void observe(Sprite & observer);
		void ignore(Sprite & observer);

		void signal(const std::string & sig, const Sprite & sprite);

		void inject();

	private:
		Script * script;

		NoneCollisionStrategy none_strategy;
		RectangularCollisionStrategy rectangular_strategy;
		CircularCollisionStrategy circular_strategy;
		PixelCollisionStrategy pixel_strategy;

		CollisionStrategy * collision_strategy;
		std::list<Sprite *> observers;

		std::map<std::string, Sheet *> sheets;
		std::string state;

		unsigned int frame;

		unsigned int script_interval;
		unsigned int observer_interval;

		float frame_timer;
		float script_timer;
		float observer_timer;
};
#endif

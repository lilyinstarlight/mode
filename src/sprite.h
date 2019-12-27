#ifndef SPRITE_H
#define SPRITE_H
#include <deque>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "collision.h"
#include "drawable.h"
#include "script.h"
#include "sheet.h"

class Sprite : public Drawable {
	public:
		Sprite(const std::string & name, bool dead = false);
		Sprite(const Sprite & s);

		virtual ~Sprite();

		Sprite() = delete;
		Sprite & operator=(const Sprite &) = delete;

		virtual void load();

		virtual void dispatch(const SDL_Event & event);
		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int ticks, World & world);

		virtual bool is_alive() const { return alive;  }
		virtual void revive();
		virtual void kill();

		virtual const Image * get_image() const;

		virtual int get_width() const;
		virtual int get_height() const;

		virtual const SDL_Surface * get_surface() const;

		std::string get_direction() const         { return direction; }
		void set_direction(const std::string & d) { direction = d;    }

		std::string get_state() const;
		void set_state(const std::string & s);

		std::string peek_state() const;
		std::string pop_state();
		void clear_state();
		void push_state(const std::string & s);

		std::vector<std::string> get_full_state() const;

		void observe(Sprite & observer);
		void ignore(Sprite & observer);

		const std::list<Sprite *> & get_observers() const { return observers; }

		void signal(const std::string & sig);
		void send(const std::string & sig, Sprite & sprite);

		void inject();

	protected:
		Script * script;

		NoneCollisionStrategy none_strategy;
		RectangularCollisionStrategy rectangular_strategy;
		CircularCollisionStrategy circular_strategy;
		PixelCollisionStrategy pixel_strategy;

		CollisionStrategy * collision_strategy;
		std::list<Sprite *> observers;

	private:
		std::unordered_map<std::string, Sheet *> sheets;

		std::string direction;
		std::deque<std::pair<std::string, unsigned int>> state;

		bool alive;

		unsigned int observer_interval;

		float frame_timer;
		float observer_timer;
};
#endif

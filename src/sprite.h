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
		const Sprite & operator=(const Sprite &) = delete;

		virtual void load();

		virtual void dispatch(const SDL_Event & event);
		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int ticks, World & world);

		virtual bool is_alive() const { return _alive;  }
		virtual void revive();
		virtual void kill();

		virtual const Image * get_image() const;

		virtual int get_width() const;
		virtual int get_height() const;

		virtual const SDL_Surface * get_surface() const;

		std::string get_direction() const                 { return _direction;      }
		void set_direction(const std::string & direction) { _direction = direction; }

		std::string get_state() const;
		void set_state(const std::string & state);

		std::string peek_state() const;
		std::string pop_state();
		void clear_state();
		void push_state(const std::string & state);

		std::vector<std::string> get_full_state() const;

		void observe(Sprite & observer);
		void ignore(Sprite & observer);

		const CollisionStrategy & get_collision_strategy() const { return *_collision_strategy; }
		const std::list<Sprite *> & get_observers() const { return _observers; }

		void signal(const std::string & sig);
		void send(const std::string & sig, Sprite & sprite);

		void inject();

	private:
		Script * _script;

		NoneCollisionStrategy _none_strategy;
		RectangularCollisionStrategy _rectangular_strategy;
		CircularCollisionStrategy _circular_strategy;
		PixelCollisionStrategy _pixel_strategy;

		CollisionStrategy * _collision_strategy;
		std::list<Sprite *> _observers;

		std::unordered_map<std::string, Sheet *> _sheets;

		std::string _direction;
		std::deque<std::pair<std::string, unsigned int>> _state;

		bool _alive;

		unsigned int _observer_interval;

		float _frame_timer;
		float _observer_timer;
};
#endif

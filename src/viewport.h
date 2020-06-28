#ifndef VIEWPORT_H
#define VIEWPORT_H
#include "drawable.h"
#include "vector2f.h"
#include "world.h"

class Viewport {
	public:
		Viewport();
		~Viewport() {}

		Viewport(const Viewport &) = delete;
		const Viewport & operator=(const Viewport &) = delete;

		void dispatch(const SDL_Event &) {}
		void draw() const {}
		void update(unsigned int ticks, World & world);

		int get_height() const { return _height; }
		int get_width() const  { return _width; }

		const Vector2f & get_position() const        { return _position;     }
		void set_position(const Vector2f & position) { _position = position; }

		float get_x() const  { return _position[0]; }
		void  set_x(float x) { _position[0] = x; }
		float get_y() const  { return _position[1]; }
		void  set_y(float y) { _position[1] = y; }

		void track(const Drawable * obj)      { _tracking = obj;  };
		const Drawable * get_tracking() const { return _tracking; }

	private:
		Vector2f _position;
		int _width, _height;

		const Drawable * _tracking;
};
#endif

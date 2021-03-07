#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <string>

#include <SDL.h>

#include "image.h"
#include "vector2f.h"

class World;

class Drawable {
	public:
		Drawable(const std::string & name, const std::string & type, const Vector2f & position, float rotation, const Vector2f & velocity, float scale, int index) : _name(name), _type(type), _position(position), _rotation(rotation), _velocity(velocity), _scale(scale), _index(index) {};
		Drawable(const Drawable & d) : _name(d._name), _type(d._type), _position(d._position), _rotation(d._rotation), _velocity(d._velocity), _scale(d._scale), _index(d._index) {};

		const Drawable & operator=(const Drawable &) = delete;

		virtual ~Drawable() {}

		virtual void load() = 0;

		virtual void dispatch(const SDL_Event & event) = 0;
		virtual void draw(const Viewport & viewport) const = 0;
		virtual void update(unsigned int ticks, World & world) = 0;

		virtual int get_width() const = 0;
		virtual int get_height() const = 0;

		virtual const SDL_Surface * get_surface() const = 0;
		virtual const Image * get_image() const = 0;

		const std::string & get_name() const    { return _name; }
		void set_name(const std::string & name) { _name = name; }

		const std::string & get_type() const    { return _type; }
		void set_type(const std::string & type) { _type = type; }

		const Vector2f & get_position() const        { return _position;     }
		Vector2f & get_mutable_position()            { return _position;     }
		void set_position(const Vector2f & position) { _position = position; }

		float get_x() const  { return _position[0]; }
		void  set_x(float x) { _position[0] = x;    }

		float get_y() const  { return _position[1]; }
		void  set_y(float y) { _position[1] = y;    }

		float get_rotation() const         { return _rotation;     }
		void  set_rotation(float rotation) { _rotation = rotation; }

		const Vector2f & get_velocity() const         { return _velocity;     }
		Vector2f & get_mutable_velocity()             { return _velocity;     }
		void  set_velocity(const Vector2f & velocity) { _velocity = velocity; }

		float get_velocity_x() const   { return _velocity[0]; }
		void  set_velocity_x(float vx) { _velocity[0] = vx;   }

		float get_velocity_y() const   { return _velocity[1]; }
		void  set_velocity_y(float vy) { _velocity[1] = vy;   }

		float get_scale() const      { return _scale;  }
		void  set_scale(float scale) { _scale = scale; }

		int   get_index() const      { return _index;  }
		void  set_index(float index) { _index = index; }

	private:
		std::string _name;
		std::string _type;
		Vector2f _position;
		float _rotation;
		Vector2f _velocity;
		float _scale;
		int _index;
};

class DrawablePointerCompare {
	public:
		bool operator() (const Drawable * left, const Drawable * right) const {
			if (left->get_index() == right->get_index())
				return left < right;

			return left->get_index() < right->get_index();
		}
};
#endif

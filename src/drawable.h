#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <string>

#include <SDL.h>

#include "image.h"
#include "vector2f.h"

class World;

class Drawable {
	public:
		Drawable(const std::string & name, const std::string & type, const Vector2f & position, float rotation, const Vector2f & velocity, float scale, int index) : name(name), type(type), position(position), rotation(rotation), velocity(velocity), scale(scale), index(index) {};

		virtual ~Drawable() {}

		virtual void load() = 0;

		virtual void dispatch(const SDL_Event & event) = 0;
		virtual void draw(const Viewport & viewport) const = 0;
		virtual void update(unsigned int ticks, World & world) = 0;

		virtual int get_width() const = 0;
		virtual int get_height() const = 0;

		virtual const SDL_Surface * get_surface() const = 0;
		virtual const Image * get_image() const = 0;

		const std::string & get_name() const { return name; }
		void set_name(const std::string & n) { name = n;    }

		const std::string & get_type() const { return type; }
		void set_type(const std::string & t) { type = t;    }

		const Vector2f & get_position() const    { return position; }
		void  set_position(const Vector2f & pos) { position = pos;  }

		float get_x() const  { return position[0]; }
		void  set_x(float x) { position[0] = x;    }

		float get_y() const  { return position[1]; }
		void  set_y(float y) { position[1] = y;    }

		float get_rotation() const    { return rotation; }
		void  set_rotation(float rot) { rotation = rot;  }

		const Vector2f & get_velocity() const    { return velocity; }
		void  set_velocity(const Vector2f & vel) { velocity = vel;  }

		float get_velocity_x() const      { return velocity[0];  }
		void  set_velocity_x(float vel_x) { velocity[0] = vel_x; }

		float get_velocity_y() const      { return velocity[1];  }
		void  set_velocity_y(float vel_y) { velocity[1] = vel_y; }

		float get_scale() const  { return scale; }
		void  set_scale(float s) { scale = s;    }

		int   get_index() const  { return index; }
		void  set_index(float i) { index = i;    }

	private:
		std::string name;
		std::string type;
		Vector2f position;
		float rotation;
		Vector2f velocity;
		float scale;
		int index;
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

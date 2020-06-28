#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <string>

#include "drawable.h"
#include "viewport.h"

class Background : public Drawable {
	public:
		enum class Tile { NONE, X, Y, BOTH };

		Background(const std::string & name);

		Background(const Background & b);

		virtual ~Background() {}

		const Background & operator=(const Background &) = delete;

		virtual void load() {}

		virtual void dispatch(const SDL_Event &) {}
		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int, World &) {}

		int get_width() const  { return _image->get_width();  }
		int get_height() const { return _image->get_height(); }

		int get_factor() const      { return _factor;   }
		void set_factor(int factor) { _factor = factor; }

		Tile get_tile() const { return _tile; }

		virtual const SDL_Surface * get_surface() const { return _image->get_surface(); };
		virtual const Image * get_image() const { return _image; };

	private:
		float _factor;
		Tile _tile;

		const Image * _image;
};
#endif

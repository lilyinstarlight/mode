#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <string>

#include "drawable.h"
#include "viewport.h"

class Background : public Drawable {
	public:
		enum Tile { NONE, X, Y, BOTH };

		Background(const std::string & name);

		Background(const Background & b);

		virtual ~Background() {}

		const Background & operator=(const Background & b) = delete;

		void dispatch(const SDL_Event &) {}
		void draw(const Viewport & viewport) const;
		void update(unsigned int) {}

		int get_width() const  { return image->get_width();  }
		int get_height() const { return image->get_height(); }

		int get_factor() const { return factor; }
		void set_factor(int f) { factor = f;    }

		virtual const SDL_Surface * get_surface() const { return image->get_surface(); };
		virtual const Image * get_image() const { return image; };

	private:
		float factor;
		Tile tile;

		const Image * image;
};
#endif

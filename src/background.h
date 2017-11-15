#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <string>

#include "drawable.h"
#include "script.h"
#include "viewport.h"

class Background : public Drawable {
	public:
		Background(const std::string & name);
		virtual ~Background() {}

		Background(const Background & b) = delete;
		const Background & operator=(const Background & b) = delete;

		void update(unsigned int ticks);
		void draw(const Viewport & viewport) const;

		int get_width() const  { return width;  }
		int get_height() const { return height; }

		virtual const SDL_Surface * get_surface() const { return image->get_surface(); };
		virtual const Image * get_image() const { return image; };
	private:

		int width, height;
		float factor;

		const Image * image;

	friend Script;
};
#endif

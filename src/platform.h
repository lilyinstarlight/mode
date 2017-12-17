#ifndef PLATFORM_H
#define PLATFORM_H
#include <string>

#include "drawable.h"
#include "viewport.h"

class Platform : public Sprite {
	public:
		enum Tile { NONE, X, Y, BOTH };

		Platform(const std::string & name);
		Platform(const Platform & p);

		virtual ~Platform();

		const Platform & operator=(const Platform & p) = delete;

		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int ticks, World & world);

		void set_width(int width)   { size.w = width; }
		int get_width() const       { return size.w; }
		void set_height(int height) { size.h = height; }
		int get_height() const      { return size.h; }

		virtual const SDL_Surface * get_surface() const { return surface; };

	private:
		Tile tile;

		SDL_Surface * surface;
		SDL_Rect size;
};
#endif

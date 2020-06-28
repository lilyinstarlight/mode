#ifndef PLATFORM_H
#define PLATFORM_H
#include <string>

#include "body.h"
#include "viewport.h"
#include "world.h"

class Platform : public Body {
	public:
		enum class Tile { NONE, X, Y, BOTH };

		Platform(const std::string & name);
		Platform(const Platform & p);

		virtual ~Platform() {}

		const Platform & operator=(const Platform &) = delete;

		virtual void draw(const Viewport & viewport) const;

		void set_width(int width)   { _size.w = width; }
		int get_width() const       { return _size.w; }
		void set_height(int height) { _size.h = height; }
		int get_height() const      { return _size.h; }

		Tile get_tile() const       { return _tile; }

	private:
		Tile _tile;

		SDL_Rect _size;
};
#endif

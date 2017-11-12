#ifndef CONSOLE_H
#define CONSOLE_H
#include <string>

#include <SDL.h>

#include "drawable.h"
#include "input.h"

class Console : public Drawable {
	public:
		static Console & get_instance();

		Console(const Console & console) = delete;
		const Console & operator=(const Console & console) = delete;

		virtual void draw() const;
		virtual void update(unsigned int ticks);

		virtual int get_width() const { return size.w; };
		virtual int get_height() const { return size.h; };

		virtual const SDL_Surface * get_surface() const { return surface; };
		virtual const Image * get_image() const { return nullptr; };

	private:
		Console();
		~Console() {}

		bool open;
		std::string command;

		SDL_Surface * surface;

		int padding_bottom;
		int padding_left;
		int padding_font;

		SDL_Rect size;
};
#endif

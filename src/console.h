#ifndef CONSOLE_H
#define CONSOLE_H
#include <string>

#include <SDL2/SDL.h>

#include "drawable.h"
#include "input.h"
#include "viewport.h"

class Console : public Drawable {
	public:
		Console();
		Console(const Console & console);

		virtual ~Console();

		const Console & operator=(const Console & console) = delete;

		virtual void load() {}

		virtual void dispatch(const SDL_Event & event);
		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int, World &) {}

		virtual int get_width() const { return 0; }
		virtual int get_height() const { return 0; }

		virtual const SDL_Surface * get_surface() const { return surface; };
		virtual const Image * get_image() const { return nullptr; };

		void open()  { opened = true;  }
		void close() { opened = false; }
		void toggle() { opened = !opened; }

		bool is_open() const { return opened; }

	private:
		Script * script;

		bool opened;
		std::string command;
		std::string result;

		SDL_Surface * surface;

		int padding_bottom;
		int padding_left;
		int padding_font;
};
#endif

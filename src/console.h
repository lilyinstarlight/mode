#ifndef CONSOLE_H
#define CONSOLE_H
#include <string>

#include <SDL.h>

#include "drawable.h"
#include "input.h"

class Console : public Drawable {
	public:
		static Console & get_instance();

		~Console() {}

		Console(const Console & console) = delete;
		const Console & operator=(const Console & console) = delete;

		virtual void dispatch(const SDL_Event & event);
		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int) {}

		virtual int get_width() const { return 0; }
		virtual int get_height() const { return 0; }

		virtual const SDL_Surface * get_surface() const { return surface; };
		virtual const Image * get_image() const { return nullptr; };

		void open()  { opened = true;  }
		void close() { opened = false; }
		void toggle() { opened = !opened; }

		bool is_open() const { return opened; }

	private:
		Console();

		bool opened;
		std::string command;
		std::string result;

		SDL_Surface * surface;

		int padding_bottom;
		int padding_left;
		int padding_font;
};
#endif

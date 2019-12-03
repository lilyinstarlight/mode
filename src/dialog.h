#ifndef DIALOG_H
#define DIALOG_H
#include <string>

#include <SDL2/SDL.h>

#include "drawable.h"
#include "image.h"
#include "input.h"

class Dialog : public Drawable {
	public:
		Dialog(const std::string & name, const std::string & text = "", bool top = false, bool open = true);
		Dialog(const Dialog & dialog);

		virtual ~Dialog() {}

		const Dialog & operator=(const Dialog &) = delete;

		virtual void load() {}

		virtual void dispatch(const SDL_Event &) {}
		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int, World &);

		virtual int get_width() const { return size.w; }
		virtual int get_height() const { return size.h; }

		virtual const SDL_Surface * get_surface() const { return text; }
		virtual const Image * get_image() const { return nullptr; }

		const std::string & get_string() const { return str; }
		void set_string(const std::string & s) { str = s;    }

		void open()  { opened = true;  }
		void close() { opened = false; }
		void toggle() { opened = !opened; }

		bool is_open() const { return opened; }

	private:
		bool above;

		bool opened;

		int padding_text;

		std::string str;
		std::string font;
		SDL_Color color;
		SDL_Color box;
		SDL_Color border;
		int border_width;

		SDL_Surface * text;
		SDL_Rect size;
};
#endif

#ifndef DIALOG_H
#define DIALOG_H
#include <string>

#include <SDL.h>

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

		virtual int get_width() const { return _size.w; }
		virtual int get_height() const { return _size.h; }

		virtual const SDL_Surface * get_surface() const { return _text; }
		virtual const Image * get_image() const { return nullptr; }

		const std::string & get_string() const { return _str; }
		void set_string(const std::string & s) { _str = s;    }

		void open()  { _opened = true;  }
		void close() { _opened = false; }
		void toggle() { _opened = !_opened; }

		bool is_open() const { return _opened; }

	private:
		bool _above;

		bool _opened;

		int _padding_text;

		std::string _str;
		std::string _font;
		SDL_Color _color;
		SDL_Color _box;
		SDL_Color _border;
		int _border_width;

		SDL_Surface * _text;
		SDL_Rect _size;
};
#endif

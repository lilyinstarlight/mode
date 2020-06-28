#ifndef CONSOLE_H
#define CONSOLE_H
#include <string>

#include <SDL.h>

#include "drawable.h"
#include "input.h"
#include "viewport.h"

class Console : public Drawable {
	public:
		Console();
		Console(const Console & console);

		virtual ~Console();

		const Console & operator=(const Console &) = delete;

		virtual void load() {}

		virtual void dispatch(const SDL_Event & event);
		virtual void draw(const Viewport &) const;
		virtual void update(unsigned int, World &);

		virtual int get_width() const { return 0; }
		virtual int get_height() const { return 0; }

		virtual const SDL_Surface * get_surface() const { return _text; };
		virtual const Image * get_image() const { return nullptr; };

		void open()   { _opened = true;     }
		void close()  { _opened = false;    }
		void toggle() { _opened = !_opened; }

		bool is_open() const { return _opened; }

	private:
		Script * _script;

		bool _opened;
		std::string _command;
		int _status;
		std::string _result;
		std::string _prompt;

		std::string _font;
		SDL_Color _color;
		SDL_Color _box;

		SDL_Surface * _text;
		SDL_Rect _size;

		int _padding_bottom;
		int _padding_left;
		int _padding_text;
};
#endif

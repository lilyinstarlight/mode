#ifndef EDITOR_H
#define EDITOR_H
#include <deque>
#include <string>

#include <SDL.h>

#include "drawable.h"
#include "input.h"
#include "script.h"
#include "viewport.h"

class Editor : public Drawable {
	public:
		Editor();
		Editor(const Editor & editor);

		virtual ~Editor() {}

		const Editor & operator=(const Editor &) = delete;

		virtual void load() {}

		virtual void dispatch(const SDL_Event & event);
		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int, World &) {}

		virtual int get_width() const  { return 0; }
		virtual int get_height() const { return 0; }

		virtual const SDL_Surface * get_surface() const { return nullptr; };
		virtual const Image * get_image() const { return nullptr; };

		void open(Script & s);
		void close();

		bool is_open() const { return _script; }

	private:
		void refresh();

		Script * _script;
		std::string _result;

		std::string _view;
		std::deque<std::deque<char>> _buffers;
		std::deque<std::deque<char>>::iterator _buffer;
		std::deque<char>::iterator _pos;

		int _columns;
		int _rows;

		int _top;

		std::string _font;
		SDL_Color _color;
		SDL_Color _box;

		int _padding_top;
		int _padding_left;
		int _padding_font;
};
#endif

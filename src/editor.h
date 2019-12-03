#ifndef EDITOR_H
#define EDITOR_H
#include <deque>
#include <string>

#include <SDL2/SDL.h>

#include "drawable.h"
#include "input.h"
#include "script.h"
#include "viewport.h"

class Editor : public Drawable {
	public:
		Editor();
		Editor(const Editor & editor);

		virtual ~Editor() {}

		const Editor & operator=(const Editor & editor) = delete;

		virtual void load() {}

		virtual void dispatch(const SDL_Event & event);
		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int, World &) {}

		virtual int get_width() const { return 0; }
		virtual int get_height() const { return 0; }

		virtual const SDL_Surface * get_surface() const { return nullptr; };
		virtual const Image * get_image() const { return nullptr; };

		void open(Script & s);
		void close();

		bool is_open() const { return script; }

	private:
		void refresh();

		Script * script;
		std::string result;

		std::string view;
		std::deque<std::deque<char>> buffers;
		std::deque<std::deque<char>>::iterator buffer;
		std::deque<char>::iterator pos;

		int columns;
		int rows;

		int top;

		int padding_top;
		int padding_left;
		int padding_font;
};
#endif

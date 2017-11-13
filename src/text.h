#ifndef _TEXT_H
#define _TEXT_H
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

class Text {
	public:
		static Text & get_instance();

		Text(const Text &) = delete;
		Text& operator=(const Text &) = delete;

		void write(SDL_Renderer * renderer, const std::string & text, int x, int y, SDL_Color color) const;

		int get_size() const { return size; }
	private:
		Text();
		~Text();

		std::string path;
		TTF_Font * font;
		int size;
};
#endif

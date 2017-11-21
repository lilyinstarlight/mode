#ifndef TEXT_H
#define TEXT_H
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

class Text {
	public:
		static Text & get_instance();

		~Text();

		Text(const Text &) = delete;
		const Text & operator=(const Text &) = delete;

		void write(SDL_Renderer * renderer, const std::string & text, int x, int y, SDL_Color color) const;

		int get_size() const { return size; }
	private:
		Text();

		int write_line(SDL_Renderer * renderer, const std::string & text, int x, int y, SDL_Color color) const;
		std::vector<std::string> split(const std::string & string, char delimiter) const;

		std::string path;
		TTF_Font * font;
		int size;
		int padding;
};
#endif

#ifndef TEXT_H
#define TEXT_H
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Text {
	public:
		static Text & get_instance();

		~Text();

		Text(const Text &) = delete;
		const Text & operator=(const Text &) = delete;

		void write(SDL_Renderer * renderer, const std::string & text, int x, int y, float rotation, SDL_Color color) const;

		SDL_Surface * write(const std::string & text, SDL_Color color) const;
		void render(SDL_Renderer * renderer, SDL_Surface * text, int x, int y, float rotation) const;
		void destroy(SDL_Surface * text) const;

		int get_size() const { return size; }
	private:
		Text();

		std::vector<std::string> split(const std::string & string, char delimiter) const;

		std::string path;
		TTF_Font * font;
		int size;
};
#endif

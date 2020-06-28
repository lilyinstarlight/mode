#ifndef TEXT_H
#define TEXT_H
#include <string>
#include <vector>
#include <unordered_map>

#include <SDL.h>
#include <SDL_ttf.h>

class Text {
	public:
		static Text & get_instance();

		~Text();

		Text(const Text &) = delete;
		const Text & operator=(const Text &) = delete;

		void write(SDL_Renderer * renderer, const std::string & name, const std::string & text, int x, int y, float rotation, SDL_Color color);

		SDL_Surface * write(const std::string & name, const std::string & text, SDL_Color color);
		void render(SDL_Renderer * renderer, SDL_Surface * text, int x, int y, float rotation) const;
		void destroy(SDL_Surface * text) const;

	private:
		Text();

		std::vector<std::string> split(const std::string & string, char delimiter) const;

		std::string _path;
		std::unordered_map<std::string, TTF_Font *> _fonts;
};
#endif

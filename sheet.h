#ifndef SPRITESHEET_H
#define SPRITESHEET_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

class SpriteSheet{
	public:
		SpriteSheet(SDL_Surface * s int width, int height, const NonOwningT) : src(s), view{0, 0, width, height}, rows(s->h/height), columns(s->w/width), frames(rows*columns), owning(false) {}

		SpriteSheet(SDL_Surface * s, int width, int height) : SpriteSheet(s, width, height, NonOwning) {
			owning = true;
		}

		~SpriteSheet() {
			if (owning)
				SDL_FreeSurface(src);
		}

		unsigned int get_rows() const    { return rows; }
		unsigned int get_columns() const { return columns; }
		unsigned int get_frames() const  { return frames; }

		SDL_Surface * get(unsigned int i, unsigned int j);
		SDL_Surface * get(unsigned int s);

		SDL_Surface* crop(SDL_Surface * surface, const SDL_Rect & view);

		SDL_Surface * operator()(unsigned i, unsigned j) {
			return get(i, j);
		}
		SDL_Surface * operator[](unsigned c) {
			return get(c);
		}

	private:
		SDL_Surface * src;
		SDL_Rect view;
		unsigned rows, columns, frames;

		bool owning;
};
#endif

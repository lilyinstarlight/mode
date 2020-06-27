#ifndef SPRITESHEET_H
#define SPRITESHEET_H
#include <SDL.h>
#include <SDL_image.h>

class SpriteSheet {
	public:
		SpriteSheet(SDL_Surface * s, int width, int height) : src(s), view{0, 0, width, height}, rows(s->h/height), columns(s->w/width), frames(rows*columns) {}

		~SpriteSheet() {}

		SpriteSheet(const SpriteSheet & ss) : src(ss.src), view(ss.view), rows(ss.rows), columns(ss.columns), frames(ss.frames) {}
		const SpriteSheet & operator=(const SpriteSheet &) = delete;

		unsigned int get_rows() const    { return rows; }
		unsigned int get_columns() const { return columns; }
		unsigned int get_frames() const  { return frames; }

		SDL_Surface * get(unsigned int i, unsigned int j);
		SDL_Surface * get(unsigned int s);

		SDL_Surface * crop(const SDL_Surface * surface, const SDL_Rect & view);

	private:
		SDL_Surface * src;
		SDL_Rect view;
		unsigned int rows, columns, frames;
};
#endif

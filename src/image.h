#ifndef IMAGE_H
#define IMAGE_H
#include <SDL.h>

class Image {
	public:
		Image(SDL_Surface * surface, SDL_Texture * texture);

		Image(const Image & image);
		Image & operator=(const Image & image);

		void draw(int x, int y) const;
		void draw(int x, int y, float scale) const;
		void draw(int sx, int sy, int dx, int dy) const;

		inline int get_width()  const { return surface->w; }
		inline int get_height() const { return surface->h; }
		SDL_Surface* get_surface() const { return surface; }

	private:
		SDL_Renderer * renderer;
		SDL_Surface * surface;
		SDL_Texture * texture;
		SDL_Rect view;

		Image();
};
#endif

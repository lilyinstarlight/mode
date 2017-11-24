#ifndef IMAGE_H
#define IMAGE_H
#include <SDL.h>

class Viewport;

class Image {
	public:
		Image(SDL_Surface * surface, SDL_Texture * texture);

		Image(const Image & image);
		Image & operator=(const Image & image);

		void draw(const Viewport & viewport, int x, int y) const;
		void draw(const Viewport & viewport, int x, int y, float scale) const;
		void draw(const SDL_Rect & src, const SDL_Rect & dst) const;

		inline int get_width()  const { return surface->w; }
		inline int get_height() const { return surface->h; }
		SDL_Surface* get_surface() const { return surface; }

	private:
		SDL_Renderer * renderer;
		SDL_Surface * surface;
		SDL_Texture * texture;
};
#endif

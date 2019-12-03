#ifndef IMAGE_H
#define IMAGE_H
#include <SDL2/SDL.h>

class Viewport;

class Image {
	public:
		Image(SDL_Surface * surface, SDL_Texture * texture);
		Image(const Image & image);

		Image & operator=(const Image & image) = delete;

		void draw(const Viewport & viewport, int x, int y) const;
		void draw(const Viewport & viewport, int x, int y, float rotation, float scale) const;
		void draw(const SDL_Rect & src, const SDL_Rect & dst, float rotation) const;

		inline int get_width()  const { return surface->w; }
		inline int get_height() const { return surface->h; }
		SDL_Surface* get_surface() const { return surface; }

	private:
		SDL_Renderer * renderer;
		SDL_Surface * surface;
		SDL_Texture * texture;
};
#endif

#ifndef IMAGE_H
#define IMAGE_H
#include <SDL.h>

class Viewport;

class Image {
	public:
		Image(SDL_Surface * surface, SDL_Texture * texture);
		Image(const Image & image);

		const Image & operator=(const Image &) = delete;

		void draw(const Viewport & viewport, int x, int y) const;
		void draw(const Viewport & viewport, int x, int y, float rotation, float scale) const;
		void draw(const SDL_Rect & src, const SDL_Rect & dst, float rotation) const;

		inline int get_width() const  { return _surface->w; }
		inline int get_height() const { return _surface->h; }
		const SDL_Surface * get_surface() const { return _surface; }

	private:
		SDL_Renderer * _renderer;
		SDL_Surface * _surface;
		SDL_Texture * _texture;
};
#endif

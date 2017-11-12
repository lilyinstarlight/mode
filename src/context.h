#ifndef CONTEXT_H
#define CONTEXT_H
#include <SDL.h>

class Context {
	public:
		static Context & get_instance();

		Context(const Context & context) = delete;
		const Context & operator=(const Context & context) = delete;

		SDL_Window * get_window() const { return window; }
		SDL_Renderer * get_renderer() const { return renderer; }

	private:
		Context();
		~Context();

		SDL_Window * window;
		SDL_Renderer * renderer;
};
#endif

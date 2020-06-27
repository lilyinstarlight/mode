#ifndef CONTEXT_H
#define CONTEXT_H
#include <string>

#include <SDL.h>

class Context {
	public:
		static Context & get_instance();

		~Context();

		Context(const Context &) = delete;
		const Context & operator=(const Context &) = delete;

		void reload();

		SDL_Window * get_window() const { return window; }
		SDL_Renderer * get_renderer() const { return renderer; }

	private:
		Context();

		std::string icon_path;

		SDL_Window * window;
		SDL_Renderer * renderer;
};
#endif

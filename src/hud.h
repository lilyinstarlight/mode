#ifndef HUD_H
#define HUD_H
#include <string>

#include "input.h"

class HUD : public Drawable {
	public:
		static HUD & get_instance();

		HUD(const HUD & hud) = delete;
		const HUD & operator=(const HUD & hud) = delete;

		virtual void draw() const;
		virtual void update(unsigned int ticks);

		virtual int get_width() const { return size.w; };
		virtual int get_height() const { return size.h; };

		virtual const SDL_Surface * get_surface() const { return surface; };
		virtual const Image * get_image() const { return nullptr; };

	private:
		HUD();
		~HUD() {}

		bool first;
		bool open;

		SDL_Surface * surface;

		int padding_top;
		int padding_left;

		SDL_Rect size;
};
#endif

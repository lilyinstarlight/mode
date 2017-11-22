#ifndef HUD_H
#define HUD_H
#include <string>

#include "drawable.h"
#include "image.h"
#include "input.h"

class HUD : public Drawable {
	public:
		static HUD & get_instance();

		~HUD() {}

		HUD(const HUD & hud) = delete;
		const HUD & operator=(const HUD & hud) = delete;

		virtual void dispatch(const SDL_Event & event);
		virtual void draw(const Viewport & viewport) const;
		virtual void update(unsigned int ticks);

		virtual int get_width() const { return 0; }
		virtual int get_height() const { return 0; }

		virtual const SDL_Surface * get_surface() const { return surface; }
		virtual const Image * get_image() const { return nullptr; }

		const std::string & get_string() const { return str; }
		void set_string(const std::string & s) { str = s;    }

		void open()  { opened = true;  }
		void close() { opened = false; }
		void toggle() { opened = !opened; }

		bool is_open() const { return opened; }

	private:
		HUD();

		int initial;
		bool opened;

		SDL_Surface * surface;

		int padding_top;
		int padding_left;
		int padding_text;

		std::string str;
};
#endif

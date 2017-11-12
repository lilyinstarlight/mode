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

	private:
		HUD();
		~HUD() {}

		bool first;
		bool open;
};
#endif

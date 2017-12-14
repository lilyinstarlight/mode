#ifndef HUD_H
#define HUD_H
#include <string>

#include "dialog.h"

class HUD : public Dialog {
	public:
		HUD();
		HUD(const HUD & hud);

		virtual ~HUD() {}

		const HUD & operator=(const HUD & hud) = delete;

		virtual void update(unsigned int ticks, World & world);

		const std::string & get_string() const { return str; }
		void set_string(const std::string & s) { str = s;    }

	private:
		std::string replace(const std::string & text, const std::string & find, const std::string & replace);

		int initial;

		std::string str;
};
#endif

#include "clock.h"
#include "engine.h"
#include "spec.h"

#include "hud.h"

HUD::HUD() : Dialog("hud", "", true, true), initial(Spec::get_instance().get_int("hud/initial")), str(Spec::get_instance().get_str("hud/str")) {}

HUD::HUD(const HUD & hud) : Dialog(hud), initial(hud.initial), str(hud.str) {}

void HUD::update(unsigned int ticks) {
	// decrement initial counter on whether this is shown at the beginning of game or not
	if (initial > 0) {
		initial -= ticks;

		if (initial <= 0)
			close();
	}

	std::string copy(str);

	copy = replace(copy, "{fps}", std::to_string(Clock::get_instance().get_fps()));
	copy = replace(copy, "{hp}", std::to_string(Engine::get_instance().get_world().get_player().get_hp()));

	Dialog::set_string(copy);
}

std::string HUD::replace(const std::string & text, const std::string & find, const std::string & replace) {
	std::string copy(text);

	std::string::size_type pos = copy.find(find);
	while (pos != std::string::npos) {
		copy = copy.replace(pos, find.size(), replace + std::string(find.length() - replace.length(), ' '));
		pos = copy.find(find);
	}

	return copy;
}

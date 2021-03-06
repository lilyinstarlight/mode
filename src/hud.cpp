#include "clock.h"
#include "spec.h"
#include "world.h"

#include "hud.h"

HUD::HUD() : Dialog("hud", "", true, true), _initial(Spec::get_instance().get_int("hud/initial")), _str(Spec::get_instance().get_str("hud/str")) {}

HUD::HUD(const HUD & hud) : Dialog(hud), _initial(hud._initial), _str(hud._str) {}

void HUD::update(unsigned int ticks, World & world) {
	// decrement initial counter on whether this is shown at the beginning of game or not
	if (_initial > 0) {
		_initial -= ticks;

		if (_initial <= 0)
			close();
	}

	std::string copy(_str);

	copy = replace(copy, "{fps}", std::to_string(Clock::get_instance().get_fps()));
	copy = replace(copy, "{hp}", std::to_string(world.get_player().get_hp()));

	copy = replace(copy, "{free}", std::to_string(world.get_player().get_pool().get_free().size()));
	copy = replace(copy, "{used}", std::to_string(world.get_player().get_pool().get_used().size()));

	Dialog::set_string(copy);

	Dialog::update(ticks, world);
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

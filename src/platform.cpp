#include <cmath>
#include <stdexcept>

#include "context.h"
#include "spec.h"
#include "util.h"

#include "platform.h"

Platform::Platform(const std::string & name) : Body(name, true), tile(NONE), size{0, 0, Spec::get_instance().get_int(name + "/width"), Spec::get_instance().get_int(name + "/height")} {
	std::string tiling = Spec::get_instance().get_str(name + "/tile");
	if (tiling == "none")
		tile = NONE;
	else if (tiling == "x")
		tile = X;
	else if (tiling == "y")
		tile = Y;
	else if (tiling == "both")
		tile = BOTH;
	else
		throw std::runtime_error("Invalid tiling: " + Spec::get_instance().get_str(get_name() + "/tile"));
}

Platform::Platform(const Platform & p) : Body(p), tile(p.tile), size(p.size) {}

void Platform::draw(const Viewport & viewport) const {
	SDL_Rect src = {0, 0, get_image()->get_width(), get_image()->get_height()};
	SDL_Rect dst = {static_cast<int>(get_x() - viewport.get_x()), static_cast<int>(get_y() - viewport.get_y()), static_cast<int>(get_scale()*src.w), static_cast<int>(get_scale()*src.h)};

	switch (tile) {
		case NONE:
			// draw adjusted background
			get_image()->draw(src, dst, get_rotation());
			break;

		case X:
			// iterate over tiles in map
			for (int x = 0; x < size.w; x += get_image()->get_width()) {
				// draw adjusted tile
				src.w = Util::min(size.w - x, get_image()->get_width());
				dst.x = static_cast<int>(get_x() - viewport.get_x()) + x;
				dst.w = src.w;
				get_image()->draw(src, dst, get_rotation());
			}

			break;

		case Y:
			// iterate over tiles in map
			for (int y = 0; y < size.h; y += get_image()->get_height()) {
				// draw adjusted tile
				src.h = Util::min(size.h - y, get_image()->get_height());
				dst.y = static_cast<int>(get_y() - viewport.get_y()) + y;
				dst.h = src.h;
				get_image()->draw(src, dst, get_rotation());
			}

			break;

		case BOTH:
			// iterate over tiles in map
			for (int x = 0; x < size.w; x += get_image()->get_width()) {
				for (int y = 0; y < size.h; y += get_image()->get_height()) {
					// draw adjusted tile
					src.w = Util::min(size.w - x, get_image()->get_width());
					src.h = Util::min(size.h - y, get_image()->get_height());
					dst.x = static_cast<int>(get_x() - viewport.get_x()) + x;
					dst.y = static_cast<int>(get_y() - viewport.get_y()) + y;
					dst.w = src.w;
					dst.h = src.h;
					get_image()->draw(src, dst, get_rotation());
				}
			}

			break;
	}
}

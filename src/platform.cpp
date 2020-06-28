#include <cmath>
#include <stdexcept>

#include "context.h"
#include "spec.h"
#include "util.h"

#include "platform.h"

Platform::Platform(const std::string & name) : Body(name, true), _tile(Tile::NONE), _size{0, 0, Spec::get_instance().get_int(name + "/width"), Spec::get_instance().get_int(name + "/height")} {
	std::string tiling = Spec::get_instance().get_str(name + "/tile");
	if (tiling == "none")
		_tile = Tile::NONE;
	else if (tiling == "x")
		_tile = Tile::X;
	else if (tiling == "y")
		_tile = Tile::Y;
	else if (tiling == "both")
		_tile = Tile::BOTH;
	else
		throw std::runtime_error("Invalid tiling: " + Spec::get_instance().get_str(get_name() + "/tile"));
}

Platform::Platform(const Platform & p) : Body(p), _tile(p._tile), _size(p._size) {}

void Platform::draw(const Viewport & viewport) const {
	SDL_Rect src = {0, 0, get_image()->get_width(), get_image()->get_height()};
	SDL_Rect dst = {static_cast<int>(get_x() - viewport.get_x()), static_cast<int>(get_y() - viewport.get_y()), static_cast<int>(get_scale()*src.w), static_cast<int>(get_scale()*src.h)};

	switch (_tile) {
		case Tile::NONE:
			// draw adjusted background
			get_image()->draw(src, dst, get_rotation());
			break;

		case Tile::X:
			// iterate over tiles in map
			for (int x = 0; x < _size.w; x += get_image()->get_width()) {
				// draw adjusted tile
				src.w = Util::min(_size.w - x, get_image()->get_width());
				dst.x = static_cast<int>(get_x() - viewport.get_x()) + x;
				dst.w = src.w;
				get_image()->draw(src, dst, get_rotation());
			}

			break;

		case Tile::Y:
			// iterate over tiles in map
			for (int y = 0; y < _size.h; y += get_image()->get_height()) {
				// draw adjusted tile
				src.h = Util::min(_size.h - y, get_image()->get_height());
				dst.y = static_cast<int>(get_y() - viewport.get_y()) + y;
				dst.h = src.h;
				get_image()->draw(src, dst, get_rotation());
			}

			break;

		case Tile::BOTH:
			// iterate over tiles in map
			for (int x = 0; x < _size.w; x += get_image()->get_width()) {
				for (int y = 0; y < _size.h; y += get_image()->get_height()) {
					// draw adjusted tile
					src.w = Util::min(_size.w - x, get_image()->get_width());
					src.h = Util::min(_size.h - y, get_image()->get_height());
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

#include <stdexcept>

#include "imagefactory.h"

#include "background.h"

Background::Background(const std::string & name) : Drawable(name,
		Spec::get_instance().get_str(name + "/type"),
		Vector2f(Spec::get_instance().get_int(name + "/position/x"),
		         Spec::get_instance().get_int(name + "/position/y")),
		0, Vector2f(0, 0), 1, Spec::get_instance().get_int(name + "/index")
	),
	_factor(Spec::get_instance().get_float(name + "/factor")),
	_tile(Tile::NONE),
	_image(ImageFactory::get_instance().get_image(name)) {
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

Background::Background(const Background & b) : Drawable(b), _factor(b._factor), _tile(b._tile), _image(b._image) {}

void Background::draw(const Viewport & viewport) const {
	SDL_Rect src = {0, 0, _image->get_width(), _image->get_height()};
	SDL_Rect dst = {static_cast<int>(get_x()*_factor - viewport.get_x()*_factor), static_cast<int>(get_y()*_factor - viewport.get_y()*_factor), src.w, src.h};

	switch (_tile) {
		case Tile::NONE:
			// draw adjusted background
			_image->draw(src, dst, 0);
			break;

		case Tile::X:
			// iterate over tiles in map
			for (int x = static_cast<int>(get_x()*_factor - viewport.get_x()*_factor) % _image->get_width() - _image->get_width(); x < viewport.get_width(); x += _image->get_width()) {
				// draw adjusted tile
				dst.x = x;
				_image->draw(src, dst, 0);
			}

			break;

		case Tile::Y:
			// iterate over tiles in map
			for (int y = static_cast<int>(get_y()*_factor - viewport.get_y()*_factor) % _image->get_height() - _image->get_height(); y < viewport.get_height(); y += _image->get_height()) {
				// draw adjusted tile
				dst.y = y;
				_image->draw(src, dst, 0);
			}

			break;

		case Tile::BOTH:
			// iterate over tiles in map
			for (int x = static_cast<int>(get_x()*_factor - viewport.get_x()*_factor) % _image->get_width() - _image->get_width(); x < viewport.get_width(); x += _image->get_width()) {
				for (int y = static_cast<int>(get_y()*_factor - viewport.get_y()*_factor) % _image->get_height() - _image->get_height(); y < viewport.get_height(); y += _image->get_height()) {
					// draw adjusted tile
					dst.x = x;
					dst.y = y;
					_image->draw(src, dst, 0);
				}
			}

			break;
	}
}

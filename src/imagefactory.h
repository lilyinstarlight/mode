#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H
#include <unordered_map>
#include <string>
#include <vector>

#include "image.h"
#include "sheet.h"
#include "spec.h"

class ImageFactory {
	public:
		static ImageFactory & get_instance();

		~ImageFactory();

		ImageFactory(const ImageFactory &) = delete;
		const ImageFactory & operator=(const ImageFactory &) = delete;

		Image * get_image(const std::string & name);
		Sheet * get_sheet(const std::string & name);

	private:
		ImageFactory();

		SDL_Surface * crop(const SDL_Surface * surface, const SDL_Rect & rect);

		std::string _path;

		std::unordered_map<std::string, SDL_Surface *> _surfaces;
		std::unordered_map<std::string, SDL_Texture *> _textures;
		std::unordered_map<std::string, Image *> _images;

		std::unordered_map<std::string, std::vector<SDL_Surface *>> _multi_surface;
		std::unordered_map<std::string, std::vector<SDL_Texture *>> _multi_texture;
		std::unordered_map<std::string, Sheet *> _sheets;
};
#endif

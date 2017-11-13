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

		Image * get_image(const std::string & name);
		Sheet * get_sheet(const std::string & name);

	private:
		std::unordered_map<std::string, SDL_Surface *> surfaces;
		std::unordered_map<std::string, SDL_Texture *> textures;
		std::unordered_map<std::string, Image *> images;

		std::unordered_map<std::string, std::vector<SDL_Surface *>> multi_surface;
		std::unordered_map<std::string, std::vector<SDL_Texture *>> multi_texture;
		std::unordered_map<std::string, Sheet *> sheets;

		ImageFactory() : surfaces(), textures(), images(), multi_surface(), multi_texture(), sheets() {}
		ImageFactory(const ImageFactory &) = delete;
		ImageFactory& operator=(const ImageFactory &) = delete;
};
#endif

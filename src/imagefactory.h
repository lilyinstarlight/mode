#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H
#include <map>
#include <string>
#include <vector>

#include "image.h"
#include "spec.h"

class ImageFactory {
	public:
		static ImageFactory & get_instance();

		~ImageFactory();

		Image * get_image(const std::string & name);
		std::vector<Image *> get_sheet(const std::string & name);

	private:
		std::map<std::string, SDL_Surface *> surfaces;
		std::map<std::string, SDL_Texture *> textures;
		std::map<std::string, Image *> images;

		std::map<std::string, std::vector<SDL_Surface *>> multi_surface;
		std::map<std::string, std::vector<SDL_Texture *>> multi_texture;
		std::map<std::string, std::vector<Image *>> multi_image;

		ImageFactory() : surfaces(), textures(), images(), multi_surface(), multi_texture(), multi_image() {}
		ImageFactory(const ImageFactory &) = delete;
		ImageFactory& operator=(const ImageFactory &) = delete;
};
#endif

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

		Image * get_image(const std::string &);
		std::vector<Image *> get_sheet(const std::string &);

	private:
		std::map<std::string, SDL_Surface *> surfaces;
		std::map<std::string, SDL_Texture *> textures;
		std::map<std::string, Image *> images;

		std::map<std::string, std::vector<SDL_Surface *>> multi_surfaces;
		std::map<std::string, std::vector<SDL_Texture *>> multi_textures;
		std::map<std::string, std::vector<Image *>> multi_images;

		ImageFactory() : surfaces(), textures(), images(), multi_surface(), multi_texture(), multi_image() {}
		ImageFactory(const ImageFactory &) = delete;
		ImageFactory& operator=(const ImageFactory &) = delete;
};
#endif

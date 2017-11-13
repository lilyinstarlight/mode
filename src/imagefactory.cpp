#include <SDL_image.h>

#include "context.h"
#include "sheet.h"
#include "spec.h"

#include "imagefactory.h"

ImageFactory & ImageFactory::get_instance() {
	static ImageFactory image_factory;
	return image_factory;
}

ImageFactory::~ImageFactory() {
	for (std::pair<std::string, SDL_Surface *> surface : surfaces)
		SDL_FreeSurface(surface.second);

	for (std::pair<std::string, SDL_Texture *> texture : textures)
		SDL_DestroyTexture(texture.second);

	for (std::pair<std::string, Image *> image : images)
		delete image.second;

	for (std::pair<std::string, std::vector<SDL_Surface *>> surfaces : multi_surface) {
		for (SDL_Surface * surface : surfaces.second)
			SDL_FreeSurface(surface);
	}

	for (std::pair<std::string, std::vector<SDL_Texture *>> textures : multi_texture) {
		for (SDL_Texture * texture : textures.second)
			SDL_DestroyTexture(texture);
	}

	for (std::pair<std::string, std::vector<Image *>> images : multi_image) {
		for (Image * image : images.second)
			delete image;
	}
}

Image* ImageFactory::get_image(const std::string & name) {
	std::map<std::string, Image *>::const_iterator pos = images.find(name);
	if ( pos != images.end() )
		return pos->second;

	SDL_Surface * surface = IMG_Load(Spec::get_instance().get_str(name + "/file").c_str());
	if (!surface)
		throw std::string("Failed to load ") + Spec::get_instance().get_str(name + "/file");
	surfaces[name] = surface;

	bool transparency = Spec::get_instance().check(name + "/transparency");
	if (transparency) {
		int r = Spec::get_instance().get_int(name + "/transparency/r");
		int g = Spec::get_instance().get_int(name + "/transparency/g");
		int b = Spec::get_instance().get_int(name + "/transparency/b");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, r, g, b, 255));
	}

	SDL_Texture * texture = SDL_CreateTextureFromSurface(Context::get_instance().get_renderer(), surface);

	Image * image = new Image(surface, texture);
	images[name] = image;

	return image;
}


std::vector<Image *> ImageFactory::get_sheet(const std::string & name) {
	std::map<std::string, std::vector<Image*>>::const_iterator pos = multi_image.find(name);
	if (pos != multi_image.end())
		return pos->second;

	SDL_Surface * sprite_surface = IMG_Load(Spec::get_instance().get_str(name + "/file").c_str());
	if (!sprite_surface)
		throw std::string("Failed to load ") + Spec::get_instance().get_str(name + "/file");

	bool transparency = Spec::get_instance().get_bool(name + "/transparency");
	int r = 0, g = 0, b = 0;
	if (transparency) {
		r = Spec::get_instance().get_int(name + "/transparency/r");
		g = Spec::get_instance().get_int(name + "/transparency/g");
		b = Spec::get_instance().get_int(name + "/transparency/b");
	}

	unsigned frames = Spec::get_instance().get_int(name + "/frames");

	std::vector<Image *> images;
	std::vector<SDL_Surface *> surfaces;
	std::vector<SDL_Texture *> textures;

	images.reserve(frames);
	surfaces.reserve(frames);
	textures.reserve(frames);

	int width = sprite_surface->w/frames;
	int height = sprite_surface->h;

	SpriteSheet sheet(sprite_surface, width, height);

	for (unsigned int i = 0; i < sheet.get_frames(); ++i) {
		SDL_Surface * surface = sheet[i];

		if (transparency)
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(sprite_surface->format, r, g, b, 255));

		SDL_Texture* texture = SDL_CreateTextureFromSurface(Context::get_instance().get_renderer(), surface);

		surfaces.push_back(surface);
		textures.push_back(texture);
		images.push_back(new Image(surface, texture));
	}

	multi_surface[name] = surfaces;
	multi_texture[name] = textures;
	multi_image[name] = images;

	return images;
}

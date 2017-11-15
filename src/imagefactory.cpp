#include <SDL_image.h>

#include "context.h"
#include "spritesheet.h"
#include "spec.h"

#include "imagefactory.h"

ImageFactory & ImageFactory::get_instance() {
	static ImageFactory image_factory;
	return image_factory;
}

ImageFactory::ImageFactory() : path("textures"), surfaces(), textures(), images(), multi_surface(), multi_texture(), sheets() {}

ImageFactory::~ImageFactory() {
	// clean up after ourselves
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

	for (std::pair<std::string, Sheet *> sheet : sheets) {
		delete sheet.second;
	}
}

Image * ImageFactory::get_image(const std::string & name) {
	// get image if already made
	std::unordered_map<std::string, Image *>::const_iterator pos = images.find(name);
	if (pos != images.end())
		return pos->second;

	// load image from file
	SDL_Surface * surface = IMG_Load((path + "/" + Spec::get_instance().get_str(name + "/file")).c_str());
	if (!surface)
		throw std::runtime_error("Failed to load " + Spec::get_instance().get_str(name + "/file"));
	surfaces[name] = surface;

	// load transparency color
	bool transparency = Spec::get_instance().check(name + "/transparency/r");
	if (transparency) {
		int r = Spec::get_instance().get_int(name + "/transparency/r");
		int g = Spec::get_instance().get_int(name + "/transparency/g");
		int b = Spec::get_instance().get_int(name + "/transparency/b");
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, r, g, b, 255));
	}

	// create texture
	SDL_Texture * texture = SDL_CreateTextureFromSurface(Context::get_instance().get_renderer(), surface);

	// create and store image
	Image * image = new Image(surface, texture);
	images[name] = image;

	return image;
}


Sheet * ImageFactory::get_sheet(const std::string & name) {
	// get sheet if already made
	std::unordered_map<std::string, Sheet *>::const_iterator pos = sheets.find(name);
	if (pos != sheets.end())
		return pos->second;

	// load sheet from file
	SDL_Surface * sheet_surface = IMG_Load((path + "/" + Spec::get_instance().get_str(name + "/file")).c_str());
	if (!sheet_surface)
		throw std::runtime_error("Failed to load " + Spec::get_instance().get_str(name + "/file"));

	// load transparency color
	bool transparency = Spec::get_instance().check(name + "/transparency/r");
	int r = 0, g = 0, b = 0;
	if (transparency) {
		r = Spec::get_instance().get_int(name + "/transparency/r");
		g = Spec::get_instance().get_int(name + "/transparency/g");
		b = Spec::get_instance().get_int(name + "/transparency/b");
	}

	// get number of frames
	unsigned frames = Spec::get_instance().get_int(name + "/frames");

	// create vector for images, surfaces, and textures
	std::vector<Image *> images;
	std::vector<SDL_Surface *> surfaces;
	std::vector<SDL_Texture *> textures;

	images.reserve(frames);
	surfaces.reserve(frames);
	textures.reserve(frames);

	// get frame width and height
	int width = sheet_surface->w/frames;
	int height = sheet_surface->h;

	// load spritesheet
	SpriteSheet spritesheet(sheet_surface, width, height);

	for (unsigned int i = 0; i < spritesheet.get_frames(); ++i) {
		// create surface for each frame
		SDL_Surface * surface = spritesheet[i];

		// add transparency if necessary
		if (transparency)
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(sheet_surface->format, r, g, b, 255));

		// create texture
		SDL_Texture * texture = SDL_CreateTextureFromSurface(Context::get_instance().get_renderer(), surface);

		// store details
		surfaces.push_back(surface);
		textures.push_back(texture);
		images.push_back(new Image(surface, texture));
	}

	// create new sheet with frames
	Sheet * sheet = new Sheet(images, Spec::get_instance().get_int(name + "/frames"), Spec::get_instance().get_int(name + "/interval"));

	// store surfaces, textures, and sheet
	multi_surface[name] = surfaces;
	multi_texture[name] = textures;
	sheets[name] = sheet;

	return sheet;
}

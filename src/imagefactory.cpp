#include <stdexcept>

#include <SDL_image.h>

#include "context.h"
#include "spec.h"

#include "imagefactory.h"

ImageFactory & ImageFactory::get_instance() {
	static ImageFactory image_factory;
	return image_factory;
}

ImageFactory::ImageFactory() : _path("textures"), _surfaces{}, _textures{}, _images{}, _multi_surface{}, _multi_texture{}, _sheets{} {}

ImageFactory::~ImageFactory() {
	// clean up after ourselves
	for (std::pair<std::string, SDL_Surface *> surface : _surfaces)
		SDL_FreeSurface(surface.second);

	for (std::pair<std::string, SDL_Texture *> texture : _textures)
		SDL_DestroyTexture(texture.second);

	for (std::pair<std::string, Image *> image : _images)
		delete image.second;

	for (std::pair<std::string, std::vector<SDL_Surface *>> multi_surfaces : _multi_surface) {
		for (SDL_Surface * surface : multi_surfaces.second)
			SDL_FreeSurface(surface);
	}

	for (std::pair<std::string, std::vector<SDL_Texture *>> multi_textures : _multi_texture) {
		for (SDL_Texture * texture : multi_textures.second)
			SDL_DestroyTexture(texture);
	}

	for (std::pair<std::string, Sheet *> sheet : _sheets) {
		delete sheet.second;
	}
}

SDL_Surface * ImageFactory::crop(const SDL_Surface * surface, const SDL_Rect & rect) {
	// create new surface
	SDL_Surface * sub = SDL_CreateRGBSurfaceWithFormat(0, rect.w, rect.h, surface->format->BitsPerPixel, surface->format->format);

	// copy rect of old surface into new surface
	SDL_BlitSurface(const_cast<SDL_Surface *>(surface), &rect, sub, nullptr);

	return sub;
}

Image * ImageFactory::get_image(const std::string & name) {
	// get image if already made
	std::unordered_map<std::string, Image *>::const_iterator pos = _images.find(name);
	if (pos != _images.end())
		return pos->second;

	std::string filename = _path + "/" + Spec::get_instance().get_str(name + "/file");

	// load image from file
	SDL_Surface * surface = IMG_Load(filename.c_str());
	if (!surface)
		throw std::runtime_error("Failed to load image file " + filename);
	_surfaces[name] = surface;

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
	if (!texture)
		throw std::runtime_error("Failed to create texture for image file " + filename);
	_textures[name] = texture;

	// create and store image
	Image * image = new Image(surface, texture);
	_images[name] = image;

	return image;
}


Sheet * ImageFactory::get_sheet(const std::string & name) {
	// get sheet if already made
	std::unordered_map<std::string, Sheet *>::const_iterator pos = _sheets.find(name);
	if (pos != _sheets.end())
		return pos->second;

	std::string filename = _path + "/" + Spec::get_instance().get_str(name + "/file");

	// load sheet from file
	SDL_Surface * sheet_surface = IMG_Load(filename.c_str());
	if (!sheet_surface)
		throw std::runtime_error("Failed to load image file " + filename);
	_surfaces[name] = sheet_surface;

	// load transparency color
	bool transparency = Spec::get_instance().check(name + "/transparency/r");
	int r = 0, g = 0, b = 0;
	if (transparency) {
		r = Spec::get_instance().get_int(name + "/transparency/r");
		g = Spec::get_instance().get_int(name + "/transparency/g");
		b = Spec::get_instance().get_int(name + "/transparency/b");
	}

	// get number of frames
	unsigned int frames = Spec::get_instance().get_int(name + "/frames");

	// create vector for images, surfaces, and textures
	std::vector<Image *> sheet_images;
	std::vector<SDL_Surface *> sheet_surfaces;
	std::vector<SDL_Texture *> sheet_textures;

	sheet_images.reserve(frames);
	sheet_surfaces.reserve(frames);
	sheet_textures.reserve(frames);

	// get frame width and height
	int width = sheet_surface->w/frames;
	int height = sheet_surface->h;

	// sliding view to crop from
	SDL_Rect view{0, 0, width, height};

	for (unsigned int idx = 0; idx < frames; ++idx) {
		// calculate new view rectangle from the sheet
		view.x = (idx % frames)*view.w;
		view.y = (idx / frames)*view.h;

		// create surface for frame from view
		SDL_Surface * surface = crop(sheet_surface, view);

		// add transparency if necessary
		if (transparency)
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(sheet_surface->format, r, g, b, 255));

		// create texture
		SDL_Texture * texture = SDL_CreateTextureFromSurface(Context::get_instance().get_renderer(), surface);

		// store details
		sheet_surfaces.push_back(surface);
		sheet_textures.push_back(texture);
		sheet_images.push_back(new Image(surface, texture));
	}

	// create new sheet with frames
	Sheet * sheet = new Sheet(sheet_images, Spec::get_instance().get_int(name + "/frames"), Spec::get_instance().get_int(name + "/interval"), Spec::get_instance().get_bool(name + "/loop"));

	// store surfaces, textures, and sheet
	_multi_surface[name] = sheet_surfaces;
	_multi_texture[name] = sheet_textures;
	_sheets[name] = sheet;

	return sheet;
}

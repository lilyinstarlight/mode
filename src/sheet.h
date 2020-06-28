#ifndef SHEET_H
#define SHEET_H
#include <vector>

#include "image.h"

class Sheet {
	public:
		Sheet(const std::vector<Image *> & images, unsigned int frames, unsigned int interval, bool loop) : _images(images.begin(), images.end()), _frames(frames), _interval(interval), _loop(loop) {}

		~Sheet() {
			for (Image * image : _images)
				delete image;
		}

		const Image * get_image(unsigned int frame) const { return _images[frame]; }
		unsigned int get_frames() const { return _frames; }
		unsigned int get_interval() const { return _interval; }
		bool get_loop() const { return _loop; }

	private:
		std::vector<Image *> _images;
		unsigned int _frames, _interval;
		bool _loop;
};
#endif

#ifndef SHEET_H
#define SHEET_H
#include <vector>

#include "image.h"

class Sheet {
	public:
		Sheet(const std::vector<Image *> & images, unsigned int frames, unsigned int interval, bool loop) : images(images.begin(), images.end()), frames(frames), interval(interval), loop(loop) {}

		~Sheet() {
			for (Image * image : images)
				delete image;
		}

		const Image * get_image(unsigned int frame) const { return images[frame]; }
		unsigned int get_frames() const { return frames; }
		unsigned int get_interval() const { return interval; }
		bool get_loop() const { return loop; }
	private:
		std::vector<Image *> images;
		unsigned int frames, interval;
		bool loop;
};
#endif

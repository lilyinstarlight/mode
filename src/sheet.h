#ifndef SHEET_H
#define SHEET_H
#include <vector>

#include "image.h"

class Sheet {
	public:
		Sheet(const std::vector<Image *> & images, int frames, int interval, bool loop) : images(images.begin(), images.end()), frames(frames), interval(interval), loop(loop) {}

		~Sheet() {
			for (Image * image : images)
				delete image;
		}

		const Image * get_image(int frame) const { return images[frame]; }
		int get_frames() const { return frames; }
		int get_interval() const { return interval; }
		bool get_loop() const { return loop; }
	private:
		std::vector<Image *> images;
		int frames, interval;
		bool loop;
};
#endif

#ifndef WORLD_H
#define WORLD_H
class Viewport;

class World {
	public:
		World();
		virtual ~World() {}

		World(const World & w) = delete;
		const World & operator=(const World & w) = delete;

		void update(unsigned int ticks);
		void draw(const Viewport & viewport) const;

		int get_width() const  { return width;  }
		void set_width(int w)  { width = w;     }
		int get_height() const { return height; }
		void set_height(int h) { height = h;    }
	private:
		int width, height;
};
#endif

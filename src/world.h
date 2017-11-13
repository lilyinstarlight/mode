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
	private:
};
#endif

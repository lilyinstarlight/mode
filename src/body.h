#ifndef BODY_H
#define BODY_H
#include <string>

#include "sprite.h"

class World;

class Body : public Sprite {
	public:
		Body(const std::string & name, bool f = false);
		Body(const Body & b);

		const Body & operator=(const Body &) = delete;

		virtual void update(unsigned int ticks, World & world);

		float get_hardness() const   { return hardness; }
		void set_hardness(float h)   { hardness = h; }
		float get_elasticity() const { return elasticity; }
		void set_elasticity(float e) { elasticity = e; }

		bool is_fixed() const { return fixed; }

	private:
		float hardness;
		float elasticity;

		bool fixed;
};
#endif

#ifndef BODY_H
#define BODY_H
#include <string>

#include "sprite.h"

class World;

class Body : public Sprite {
	public:
		Body(const std::string & name, bool fixed = false);
		Body(const Body & b);

		const Body & operator=(const Body &) = delete;

		virtual void update(unsigned int ticks, World & world);

		float get_hardness() const        { return _hardness;     }
		void set_hardness(float hardness) { _hardness = hardness; }
		float get_elasticity() const          { return _elasticity;       }
		void set_elasticity(float elasticity) { _elasticity = elasticity; }

		bool is_fixed() const      { return _fixed;  }
		void set_fixed(bool fixed) { _fixed = fixed; }

	private:
		float _hardness;
		float _elasticity;

		bool _fixed;
};
#endif

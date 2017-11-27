#ifndef PROJECTILEPOOL_H
#define PROJECTILEPOOL_H
#include <stack>

#include "sprite.h"

class ProjectilePool {
	public:
		static ProjectilePool & get_instance();

		~ProjectilePool();

		ProjectilePool(const ProjectilePool &) = delete;
		const ProjectilePool & operator=(const ProjectilePool &) = delete;

		const Sprite & create();
		void destroy(const Sprite &);

	private:
		ProjectilePool();
};
#endif

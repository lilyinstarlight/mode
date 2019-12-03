#ifndef UTIL_H
#define UTIL_H
#include <cmath>

#include "vector2f.h"

class Util {
	public:
		static inline int max(int left, int right) {
			return right > left ? right : left;
		}

		static inline int min(int left, int right) {
			return right < left ? right : left;
		}

		static inline float distance(const Vector2f & p1, const Vector2f & p2) {
			return std::sqrt(std::pow(p2[0] - p1[0], 2) + std::pow(p2[1] - p1[1], 2));
		}
};
#endif

#ifndef VECTOR2F_H
#define VECTOR2F_H
#include <ostream>

class Vector2f {
	public:
		static constexpr float EPSILON = 0.001;

		Vector2f(float x = 0, float y = 0);
		Vector2f(const Vector2f & v);

		Vector2f & operator=(const Vector2f & v);

		bool operator==(const Vector2f & other) const;
		bool operator!=(const Vector2f & other) const;

		float & operator[](int index);
		float operator[](int index) const;


		Vector2f operator*(float scale) const;
		Vector2f operator/(float scale) const;

		Vector2f operator+(const Vector2f & other) const;
		Vector2f operator-(const Vector2f & other) const;
		Vector2f operator-() const;

		const Vector2f & operator*=(float scale);
		const Vector2f & operator/=(float scale);
		const Vector2f & operator+=(const Vector2f & other);
		const Vector2f & operator-=(const Vector2f & other);

		float get_x() const { return _x; }
		void set_x(float x) { _x = x;    }
		float get_y() const { return _y; }
		void set_y(float y) { _y = y;    }

		float magnitude() const;
		float square_magnitude() const;
		Vector2f normalize() const;
		float dot(const Vector2f &other) const;

		Vector2f clamp(const Vector2f & min, const Vector2f & max) const;

	private:
		float _x, _y;
};

Vector2f operator*(float scale, const Vector2f & v);
std::ostream & operator<<(std::ostream & out, const Vector2f & v);

#endif

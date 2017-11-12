#ifndef _VECTOR2F_H
#define _VECTOR2F_H

#include <iostream>

class Vector2f {
	public:
		static constexpr float EPSILON = 0.001;

		explicit Vector2f(float x = 0, float y = 0);

		Vector2f & operator=(const Vector2f &);

		bool operator==(const Vector2f &other) const;
		bool operator!=(const Vector2f &other) const;

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

		float magnitude() const;
		float square_magnitude() const;
		Vector2f normalize() const;
		float dot(const Vector2f &other) const;

	private:
		float v[2];
};

Vector2f operator*(float scale, const Vector2f & v);
std::ostream & operator<<(std::ostream & out, const Vector2f & v);

#endif

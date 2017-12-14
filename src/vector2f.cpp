#include <cmath>
#include <string>

#include "vector2f.h"

Vector2f::Vector2f(float vx, float vy) : x(vx), y(vy) {}

Vector2f & Vector2f::operator=(const Vector2f & rhs) {
	if (this == &rhs)
		return *this;

	x = rhs.x;
	y = rhs.y;

	return *this;
}

bool Vector2f::operator==(const Vector2f & other) const {
	return x == other.x && y == other.y;
}

bool Vector2f::operator!=(const Vector2f & other) const {
	return x != other.x || y != other.y;
}

float & Vector2f::operator[](int index) {
	switch(index) {
		case 0:
			return x;

		case 1:
			return y;

		default:
			throw std::out_of_range("Index " + std::to_string(index) + " out of range");
	}
}

float Vector2f::operator[](int index) const {
	switch(index) {
		case 0:
			return x;

		case 1:
			return y;

		default:
			throw std::out_of_range("Index " + std::to_string(index) + " out of range");
	}
}

Vector2f Vector2f::operator*(float scale) const {
	return Vector2f(x*scale, y*scale);
}

Vector2f Vector2f::operator/(float scale) const {
	if (scale < Vector2f::EPSILON && scale > -Vector2f::EPSILON)
		throw std::runtime_error("Division by near zero");

	return Vector2f(x/scale, y/scale);
}

Vector2f Vector2f::operator+(const Vector2f & other) const {
	return Vector2f(x + other.x, y + other.y);
}

Vector2f Vector2f::operator-(const Vector2f & other) const {
	return Vector2f(x - other.x, y - other.y);
}

Vector2f Vector2f::operator-() const {
	return Vector2f(-x, -y);
}

const Vector2f & Vector2f::operator*=(float scale) {
	x *= scale;
	y *= scale;

	return *this;
}

const Vector2f & Vector2f::operator/=(float scale) {
	if (scale < Vector2f::EPSILON && scale > -Vector2f::EPSILON)
		throw std::runtime_error("Division by near zero");

	x /= scale;
	y /= scale;

	return *this;
}

const Vector2f & Vector2f::operator+=(const Vector2f & other) {
	x += other.x;
	y += other.y;

	return *this;
}

const Vector2f & Vector2f::operator-=(const Vector2f & other) {
	x -= other.x;
	y -= other.y;

	return *this;
}

float Vector2f::magnitude() const {
	return sqrt(x*x + y*y);
}

float Vector2f::square_magnitude() const {
	return x*x + y*y;
}

Vector2f Vector2f::normalize() const {
	float m = std::sqrt(x*x + y*y);

	if (m < Vector2f::EPSILON && m > -Vector2f::EPSILON)
		throw std::runtime_error("Point too close in Vector2f::normalize");

	return Vector2f(x/m, y/m);
}

float Vector2f::dot(const Vector2f &other) const {
	return x*other.x + y*other.y ;
}

Vector2f Vector2f::clamp(const Vector2f & min, const Vector2f & max) const {
	Vector2f clamped;

	if (x < min.x)
		clamped.x = min.x;
	else if (x > max.x)
		clamped.x = max.x;
	else
		clamped.x = x;

	if (y < min.y)
		clamped.y = min.y;
	else if (y > max.y)
		clamped.y = max.y;
	else
		clamped.y = y;

	return clamped;
}

Vector2f operator*(float scale, const Vector2f & v) {
	return v*scale;
}

std::ostream & operator<<(std::ostream & out, const Vector2f & v) {
	return out << "(" << v[0] << ", " << v[1] << ")";
}

#include <stdexcept>

#include <cmath>
#include <string>

#include "vector2f.h"

Vector2f::Vector2f(float x, float y) : _x(x), _y(y) {}

Vector2f::Vector2f(const Vector2f & v) : _x(v._x), _y(v._y) {}

Vector2f & Vector2f::operator=(const Vector2f & v) {
	if (&v == this)
		return *this;

	_x = v._x;
	_y = v._y;

	return *this;
}

bool Vector2f::operator==(const Vector2f & other) const {
	return _x == other._x && _y == other._y;
}

bool Vector2f::operator!=(const Vector2f & other) const {
	return _x != other._x || _y != other._y;
}

float & Vector2f::operator[](int index) {
	switch(index) {
		case 0:
			return _x;

		case 1:
			return _y;

		default:
			throw std::out_of_range("Index " + std::to_string(index) + " out of range");
	}
}

float Vector2f::operator[](int index) const {
	switch(index) {
		case 0:
			return _x;

		case 1:
			return _y;

		default:
			throw std::out_of_range("Index " + std::to_string(index) + " out of range");
	}
}

Vector2f Vector2f::operator*(float scale) const {
	return Vector2f(_x*scale, _y*scale);
}

Vector2f Vector2f::operator/(float scale) const {
	if (scale < Vector2f::EPSILON && scale > -Vector2f::EPSILON)
		throw std::runtime_error("Division by near zero");

	return Vector2f(_x/scale, _y/scale);
}

Vector2f Vector2f::operator+(const Vector2f & other) const {
	return Vector2f(_x + other._x, _y + other._y);
}

Vector2f Vector2f::operator-(const Vector2f & other) const {
	return Vector2f(_x - other._x, _y - other._y);
}

Vector2f Vector2f::operator-() const {
	return Vector2f(-_x, -_y);
}

const Vector2f & Vector2f::operator*=(float scale) {
	_x *= scale;
	_y *= scale;

	return *this;
}

const Vector2f & Vector2f::operator/=(float scale) {
	if (scale < Vector2f::EPSILON && scale > -Vector2f::EPSILON)
		throw std::runtime_error("Division by near zero");

	_x /= scale;
	_y /= scale;

	return *this;
}

const Vector2f & Vector2f::operator+=(const Vector2f & other) {
	_x += other._x;
	_y += other._y;

	return *this;
}

const Vector2f & Vector2f::operator-=(const Vector2f & other) {
	_x -= other._x;
	_y -= other._y;

	return *this;
}

float Vector2f::magnitude() const {
	return sqrt(_x*_x + _y*_y);
}

float Vector2f::square_magnitude() const {
	return _x*_x + _y*_y;
}

Vector2f Vector2f::normalize() const {
	float m = std::sqrt(_x*_x + _y*_y);

	if (m < Vector2f::EPSILON && m > -Vector2f::EPSILON)
		throw std::runtime_error("Point too close in Vector2f::normalize");

	return Vector2f(_x/m, _y/m);
}

float Vector2f::dot(const Vector2f &other) const {
	return _x*other._x + _y*other._y ;
}

Vector2f Vector2f::clamp(const Vector2f & min, const Vector2f & max) const {
	Vector2f clamped;

	if (_x < min._x)
		clamped._x = min._x;
	else if (_x > max._x)
		clamped._x = max._x;
	else
		clamped._x = _x;

	if (_y < min._y)
		clamped._y = min._y;
	else if (_y > max._y)
		clamped._y = max._y;
	else
		clamped._y = _y;

	return clamped;
}

Vector2f operator*(float scale, const Vector2f & v) {
	return v*scale;
}

std::ostream & operator<<(std::ostream & out, const Vector2f & v) {
	return out << "(" << v[0] << ", " << v[1] << ")";
}

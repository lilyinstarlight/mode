#include <cmath>
#include <iostream>
#include <string>

#include "vector2f.h"

Vector2f::Vector2f(float x, float y) {
	v[0] = x;
	v[1] = y;
}

Vector2f & Vector2f::operator=(const Vector2f & rhs) {
	if (this == &rhs)
		return *this;

	v[0] = rhs[0];
	v[1] = rhs[1];

	return *this;
}

bool Vector2f::operator==(const Vector2f & other) const {
	return v[0] == other.v[0] && v[1] == other.v[1];
}

bool Vector2f::operator!=(const Vector2f & other) const {
	return v[0] != other.v[0] || v[1] != other.v[1];
}

float & Vector2f::operator[](int index) {
	return v[index];
}

float Vector2f::operator[](int index) const {
	return v[index];
}

Vector2f Vector2f::operator*(float scale) const {
	return Vector2f(v[0]*scale, v[1]*scale);
}

Vector2f Vector2f::operator/(float scale) const {
	if (scale < EPSILON && scale > -EPSILON)
		throw std::string("Division by near zero");

	return Vector2f(v[0]/scale, v[1]/scale);
}

Vector2f Vector2f::operator+(const Vector2f & other) const {
	return Vector2f(v[0] + other.v[0], v[1] + other.v[1]);
}

Vector2f Vector2f::operator-(const Vector2f & other) const {
	return Vector2f(v[0] - other.v[0], v[1] - other.v[1]);
}

Vector2f Vector2f::operator-() const {
	return Vector2f(-v[0], -v[1]);
}

const Vector2f & Vector2f::operator*=(float scale) {
	v[0] *= scale;
	v[1] *= scale;

	return *this;
}

const Vector2f & Vector2f::operator/=(float scale) {
	if (scale < EPSILON && scale > -EPSILON)
		throw std::string("Division by near zero");

	v[0] /= scale;
	v[1] /= scale;

	return *this;
}

const Vector2f & Vector2f::operator+=(const Vector2f & other) {
	v[0] += other.v[0];
	v[1] += other.v[1];

	return *this;
}

const Vector2f & Vector2f::operator-=(const Vector2f & other) {
	v[0] -= other.v[0];
	v[1] -= other.v[1];

	return *this;
}

float Vector2f::magnitude() const {
	return sqrt(v[0] * v[0] + v[1] * v[1]);
}

float Vector2f::square_magnitude() const {
	return v[0]*v[0] + v[1]*v[1];
}

Vector2f Vector2f::normalize() const {
	float m = std::sqrt(v[0]*v[0] + v[1]*v[1]);

	if (m < Vector2f::EPSILON && m > -Vector2f::EPSILON)
		throw std::string("Point too close in Vector2f::normalize");

	return Vector2f(v[0]/m, v[1]/m);
}

float Vector2f::dot(const Vector2f &other) const {
	return v[0]*other.v[0] + v[1]*other.v[1] ;
}

Vector2f operator*(float scale, const Vector2f & v) {
	return v*scale;
}

std::ostream & operator<<(std::ostream & out, const Vector2f & v) {
	return out << "(" << v[0] << ", " << v[1]  << ")";
}

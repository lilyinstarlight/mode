#ifndef VECTOR2F_H
#define VECTOR2F_H

class Vector2f {
	public:
		static constexpr float EPSILON = 0.001;

		explicit Vector2f(float vx = 0, float vy = 0);

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

		float get_x() const  { return x; }
		void set_x(float vx) { x = vx;   }
		float get_y() const  { return y; }
		void set_y(float vy) { y = vy;   }

		float magnitude() const;
		float square_magnitude() const;
		Vector2f normalize() const;
		float dot(const Vector2f &other) const;

		Vector2f clamp(const Vector2f & min, const Vector2f & max) const;

	private:
		float x, y;
};

Vector2f operator*(float scale, const Vector2f & v);
std::ostream & operator<<(std::ostream & out, const Vector2f & v);

#endif

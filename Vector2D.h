#pragma once
#include <iostream>
class Vector2D {
public:
	float x;
	float y;
	Vector2D();
	Vector2D(float x, float y);
	Vector2D& Add(const Vector2D& vec);
	Vector2D& Subtract(const Vector2D& vec);
	Vector2D& Multiply(const float k);
	Vector2D& Divide(const float k);
	Vector2D& Normalize();
	float SqrMagnitude();
	float Magnitude();
	friend Vector2D& operator+(Vector2D& v1, const Vector2D v2);
	friend Vector2D& operator-(Vector2D& v1, const Vector2D v2);
	friend Vector2D& operator*(Vector2D& v1, const float k);
	friend Vector2D& operator/(Vector2D& v1, const float k);
	Vector2D& operator=(const Vector2D& vec);
	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const float k);
	Vector2D& operator/=(const float k);
	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};
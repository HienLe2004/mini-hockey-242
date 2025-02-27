#include "Vector2D.h"
#include "math.h"
Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}
Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}
Vector2D& Vector2D::Add(const Vector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	return *this;
}
Vector2D& Vector2D::Subtract(const Vector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}
Vector2D& Vector2D::Multiply(const float k)
{
	this->x *= k;
	this->y *= k;
	return *this;
}
Vector2D& Vector2D::Divide(const float k)
{
	this->x /= k;
	this->y /= k;
	return *this;
}
Vector2D& Vector2D::Normalize()
{
	float currentLength = this->x * this->x + this->y * this->y;
	if (currentLength == 0) return *this;
	float k = 1 / currentLength;
	this->x *= sqrt(k);
	this->y *= sqrt(k);
	return *this;
}
float Vector2D::SqrMagnitude()
{
	return x * x + y * y;
}
float Vector2D::Magnitude()
{
	return sqrt(x * x + y * y);
}
Vector2D& operator+(Vector2D& v1, const Vector2D& v2)
{
	return v1.Add(v2);
}
Vector2D& operator-(Vector2D& v1, const Vector2D& v2)
{
	return v1.Subtract(v2);
}
Vector2D& operator*(Vector2D& v1, const float k)
{
	return v1.Multiply(k);
}
Vector2D& operator/(Vector2D& v1, const float k)
{
	return v1.Divide(k);
}
Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	return this->Add(vec);
}
Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	return this->Subtract(vec);
}
bool Vector2D::operator==(const Vector2D& vec) {
	return (this->x == vec.x && this->y == vec.y);
}
Vector2D& Vector2D::operator=(const Vector2D& vec)
{
	this->x = vec.x;
	this->y = vec.y;
	return *this;
}
Vector2D& Vector2D::operator*=(const float k)
{
	return this->Multiply(k);
}
Vector2D& Vector2D::operator/=(const float k)
{
	return this->Divide(k);
}
std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}
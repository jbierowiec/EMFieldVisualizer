#pragma once
#include <cmath>

struct Vector2D
{
    float x, y;
    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}

    Vector2D operator+(const Vector2D &other) const { return {x + other.x, y + other.y}; }
    Vector2D operator-(const Vector2D &other) const { return {x - other.x, y - other.y}; }
    Vector2D operator*(float scalar) const { return {x * scalar, y * scalar}; }

    Vector2D &operator+=(const Vector2D &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2D &operator-=(const Vector2D &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2D normalized() const
    {
        float mag = magnitude();
        return mag == 0 ? Vector2D() : (*this) * (1.0f / mag);
    }
    
    float magnitude() const { return std::sqrt(x * x + y * y); }
    float angle() const { return atan2(y, x) * 180.0 / M_PI; }
};

#pragma once

#include <cmath>

struct Vec2
{
  float x, y;
  Vec2() : x(0.0f), y(0.0f) {}
  Vec2(float x, float y) : x(x), y(y) {}

  Vec2 operator+(const Vec2 &v) const {
		// Add
		return Vec2(x + v.x, y + v.y);
	}

  Vec2 operator-(const Vec2 &v) const {
		// Subtract
		return Vec2(x - v.x, y - v.y);
	}

  Vec2 operator*(float d) const {
		// Multiply
		return Vec2(x * d, y * d);
	}

  Vec2 operator/(float d) const {
		// Divide
		return Vec2(x / d, y / d);
	}
};

namespace vec {
	float magSq(Vec2 a);
	float mag(Vec2 a);
	Vec2 normalized(Vec2 a);
	Vec2 multiply(Vec2 a, Vec2 b);
	float dot(const Vec2 &a, const Vec2 &b);
	float cross(const Vec2 &a, const Vec2 &b);
	Vec2 abs(Vec2 vec);
};

float randf();
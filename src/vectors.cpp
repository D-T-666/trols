#include <cmath>
#include "../include/vectors.h"

float randf(){
	// Random float from 0.0f to 1.0f
	return float(rand()) / float((RAND_MAX));
}

float vec::dot(const Vec2 &a, const Vec2 &b){
	// Dot product 2D
	return a.x * b.x + a.y * b.y;
}

float vec::cross(const Vec2 &a, const Vec2 &b){
	// Cross product 2D
	return a.x * b.y - a.y * b.x;
}

Vec2 vec::abs(Vec2 a){
	// Element-wise absolute of vector
	return Vec2(fabs(a.x), fabs(a.y));
};

Vec2 vec::multiply(Vec2 a, Vec2 b) {
	// Element-wise multiplication
	return Vec2(a.x * b.x, a.y * b.y);
}

inline float vec::magSq(Vec2 a) {
	// Magnitude squared
	return a.x * a.x + a.y * a.y;
}

float vec::mag(Vec2 a) {
	// Magniture
	return sqrt(vec::magSq(a));
}

Vec2 vec::normalized(Vec2 a) {
	// Normalized vector
	float mg = vec::mag(a);
	return Vec2(a.x / mg, a.y / mg);
}
#include "vec.h"



sf::Vector2f getVec (float R, float angle) {
	return sf::Vector2f (R * cosf (angle), R * sinf (angle));
}

inline float dot (const sf::Vector2f& a, const sf::Vector2f& b) {
	return a.x * b.x + a.y * b.y;
}



inline float cross (const sf::Vector2f& a, const sf::Vector2f& b) {
	return a.x * b.y - a.y * b.x;
}

inline sf::Vector2f cross (float a, const sf::Vector2f& v) {
	return {-a * v.y, a * v.x};
}

inline sf::Vector2f cross (const sf::Vector2f& v, float b) {
	return {b * v.y, -b * v.x};
}



inline bool equal (float a, float b) {return abs (a - b) <= dontGiveAf;}

inline int rnd (float a) {return static_cast<int> (a + 0.5f);}

inline float clamp (float min, float max, float a) {
	return (a < min)? min:
	                ((a > max)? max : a);
}

inline bool biasGreater (float a, float b) {return a >= 0.95f * b + 0.01f * a;}





float lenSqrt (const sf::Vector2f& v) {
	return v.x * v.x + v.y * v.y;
}

inline float lenSqrt (const sf::Vector2f& a, const sf::Vector2f& b) {
	const sf::Vector2f c = a - b;
	return dot (c, c);
}

inline float len (const sf::Vector2f& v) {
	return sqrtf (v.x * v.x + v.y * v.y);
}

inline float len (const sf::Vector2f& a, const sf::Vector2f& b) {
	const sf::Vector2f c = a - b;
	return sqrtf (dot (c, c));
}



inline void rotate (sf::Vector2f& v, float angle) {
	const float Cos = cosf (angle), Sin = sinf (angle), x = v.x, y = v.y;
	v = {Cos*x - Sin*y, Sin*x + Cos*y};
}

inline void normalize (sf::Vector2f& v) {
	const float L = len (v);
	if (L > dontGiveAf) { v *= 1.f / L; }
}

inline void mat2x2::set (float angle) {
	const float Cos = cosf (angle), Sin = sinf (angle);
	m00 = Cos, m01 = -Sin;
	m10 = Sin, m11 =  Cos;
}

mat2x2::mat2x2 () {}

mat2x2::mat2x2 (float m0, float m1, float m2, float m3)
: m00 (m0), m01 (m1),
  m10 (m2), m11 (m3)
{}

mat2x2::mat2x2 (float angle) {
	set (angle);
}

mat2x2::~mat2x2 () {}

inline sf::Vector2f mat2x2::axisX () {return {m00, m10};}
inline sf::Vector2f mat2x2::axisY () {return {m01, m11};}
inline mat2x2 mat2x2::T () {return mat2x2 (m00, m10, m01, m11);}



mat2x2& mat2x2::operator*= (float rhs) {
	m00 *= rhs, m01 *= rhs;
	m10 *= rhs, m11 *= rhs;
	return *this;
}

mat2x2& mat2x2::operator+= (const mat2x2& rhs) {
	m00 += rhs.m00, m01 += rhs.m01;
	m10 += rhs.m10, m11 += rhs.m11;
	return *this;
}

mat2x2 mat2x2::operator* (const mat2x2& rhs) {
	return mat2x2 (
		m00*rhs.m00 + m01*rhs.m10,   m00*rhs.m01 + m01*rhs.m11,
		m10*rhs.m00 + m11*rhs.m10,   m10*rhs.m01 + m11*rhs.m11);
}

sf::Vector2f operator*(const mat2x2& M, const sf::Vector2f& rhs){
	return {M.m00*rhs.x + M.m01*rhs.y, M.m10*rhs.x + M.m11*rhs.y};
}

mat2x2 mat2x2::operator+ (const mat2x2& rhs) {
	return mat2x2 (
		m00+rhs.m00, m01+rhs.m01,
		m10+rhs.m10, m11+rhs.m11);
}

mat2x2 operator* (const mat2x2& M, float rhs) {
	return mat2x2 (rhs*M.m00, rhs*M.m01, rhs*M.m10, rhs*M.m11);
}

mat2x2 operator* (float lhs, const mat2x2& M) {
	return mat2x2 (lhs*M.m00, lhs*M.m01, lhs*M.m10, lhs*M.m11);
}

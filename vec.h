#ifndef VEC_H
#define VEC_H
#include <SFML/Graphics.hpp>
#include <cmath>

const float dontGiveAf = 0.00001f;

sf::Vector2f getVec (float R, float angle);

float dot (const sf::Vector2f& a, const sf::Vector2f& b);
float cross (const sf::Vector2f& a, const sf::Vector2f& b);
sf::Vector2f cross (float a, const sf::Vector2f& v);
sf::Vector2f cross (const sf::Vector2f& v, float b);

float lenSqrt (const sf::Vector2f& v);
float lenSqrt (const sf::Vector2f& a, const sf::Vector2f& b);

float len (const sf::Vector2f& v);
float len (const sf::Vector2f& a, const sf::Vector2f& b);

void rotate (sf::Vector2f& v, float angle);
void normalize (sf::Vector2f& v);

bool	equal (float a, float b);
int		rnd (float a);
float	clamp (float from, float to, float a);

bool biasGreater (float a, float b);

struct mat2x2 {

	float m00 = 0.f, m01 = 0.f,
	      m10 = 0.f, m11 = 0.f;
	mat2x2 ();
	mat2x2 (float m0, float m1, float m2, float m3);
	mat2x2 (float angle);
	~mat2x2 ();
	void set (float angle);
	sf::Vector2f axisX ();
	sf::Vector2f axisY ();
	mat2x2 T();
	mat2x2& operator*= (float rhs);
	mat2x2& operator+= (const mat2x2& rhs);
	mat2x2 operator* (const mat2x2& rhs);
	//sf::Vector2f operator* (const sf::Vector2f& rhs);
	mat2x2 operator+ (const mat2x2& rhs);
};

mat2x2 operator*(const mat2x2& M, float rhs);
mat2x2 operator*(float lhs, const mat2x2& M);

sf::Vector2f operator*(const mat2x2& M, const sf::Vector2f& rhs);
//sf::Vector2f operator*(const sf::Vector2f& lhs, const mat2x2& M);


const mat2x2 E (1.f, 0.f, 0.f, 1.f);

#endif

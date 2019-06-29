#ifndef SHAPE_H
#define SHAPE_H
#include "vec.h"
#include "math_constants.h"
//#include "body.h"
#include <memory>
#include <cfloat>
#include <cassert>

struct Body;

const unsigned MaxPolyVertex = 42;




struct Shape {

	enum class type {circle_t, poly_t};

	Body* body;
	float radius;
	mat2x2 orient;

	Shape ( ) {}
	virtual void		init () = 0;
	virtual type		getType () const = 0;
	virtual void		draw (sf::RenderWindow& window) const = 0;
	virtual void		setOrient (float radians) = 0;
	virtual void		computeMass (float density) = 0;
	virtual std::shared_ptr<Shape> copy () const = 0;
};



struct Circle : public Shape {

	Circle (float r) {radius =r;}

	void init () {computeMass (1.0f);}

	std::shared_ptr<Shape> copy () const {
		return std::shared_ptr<Shape> (new Circle (radius));
	}

	type getType () const {return type::circle_t;}

	void draw (sf::RenderWindow& window) const {
		const unsigned segments = 20;
		// Draw a circle
		sf::Vertex v[segments];
		double phi = body->orient;
		float step = 2.f*static_cast<float> (m_pi) / static_cast<float> (segments);
		for (unsigned i = 0; i < segments; ++i) {
			phi += step;
			v[i].position = sf::Vector2f(cos (phi), sin (phi));
			sf::Vector2f& p = v[i].position;
			p *= radius;
			p += body->position;
		}
		window.draw (v, segments, sf::LineStrip);
		/* // Draw orient line
			sf::Vector2f r (0.f, 1.f);
			const float Cos = cos (body->orient), Sin = sin (body->orient);
			r.Set( r.x*Cos - r.y*Sin, r.x*Sin + r.y*Cos );
			r *= radius;
			r += body->position;
			v[0].position = {body->position.x, body->position.y};
			v[1].position = {r.x, r.y};
			window.draw (v, 2, sf::Line);
		// */
	}
	void setOrient (float radians) {}
	void computeMass (float density) {
		body->m = m_pi * radius * radius * density;
		body->im = (body->m) ? 1.0f / body->m : 0.0f;
		body->I = body->m * radius * radius;
		body->iI = (body->I) ? 1.0f / body->I : 0.0f;
	}
};




struct Polygon : public Shape {

	unsigned m_vertexCount;
	sf::Vector2f m_vertices[MaxPolyVertex];
	sf::Vector2f m_normals[MaxPolyVertex];

	void init () {computeMass (1.f);}

	std::shared_ptr<Shape> copy () const {
		Polygon* poly;
		std::shared_ptr<Shape> sh (poly = new Polygon ());
		poly->orient = orient;

		for (unsigned i = 0; i < m_vertexCount; ++i) {
			poly->m_vertices[i] = m_vertices[i];
			poly->m_normals[i] = m_normals[i];
		}
		poly->m_vertexCount = m_vertexCount;

		return sh;
	}

	void computeMass (float density)
	{
		// Вычисление центроида и момента инерции
		sf::Vector2f centroid (0.f, 0.f);
		float S = 0.f;
		float I = 0.f;
		const float coefInv3 = 1.f / 3.f;

		for (unsigned it = 0, jt = 1; it < m_vertexCount; ++it, ++jt) {
			// Вершины треугольника, где третяя лежит в (0, 0)
			if (jt == m_vertexCount) {jt = 0;}
			sf::Vector2f p1 (m_vertices[it]), p2 (m_vertices[jt]);
			float D = cross (p1, p2);
			float triangleS = 0.5f * D;
			S += triangleS;

			// Исп. взвешенных векторов, для уточнения реального центроида
			centroid += triangleS * coefInv3 * (p1 + p2);
			float intx2 = p1.x*p1.x + p2.x*p1.x + p2.x*p2.x;
			float inty2 = p1.y*p1.y + p2.y*p1.y + p2.y*p2.y;
			I += (0.25f * coefInv3 * D) * (intx2 + inty2);
		}
		centroid *= 1.f / S;

		// Перенос вершин к центроиду
		for (unsigned i = 0; i < m_vertexCount; ++i) {m_vertices[i] -= centroid;}
		body->m = density * S;
		body->im = (body->m) ? 1.0f / body->m : 0.0f;
		body->I = I * density;
		body->iI = body->I ? 1.0f / body->I : 0.0f;
	}

	void setOrient (float angle) {orient.set (angle);}

	type getType () const {return type::poly_t;}

	void draw (sf::RenderWindow& window) const {
		sf::Vertex v[m_vertexCount];
		for (unsigned idx = 0; idx < m_vertexCount; ++idx) {
			v[idx].position = sf::Vector2f(body->position + (orient * m_vertices[idx]));
		}
		window.draw (v, m_vertexCount, sf::LineStrip);
	}

	// Пол высоты, пол ширины
	void setBox (float hw, float hh) {
		m_vertexCount = 4;
		m_vertices[0] = {-hw,-hh};
		m_vertices[1] = { hw,-hh};
		m_vertices[2] = { hw, hh};
		m_vertices[3] = {-hw, hh};
		m_normals[0] = { 0.0f,-1.0f};
		m_normals[1] = { 1.0f, 0.0f};
		m_normals[2] = { 0.0f, 1.0f};
		m_normals[3] = {-1.0f, 0.0f};
	}

	void set (sf::Vector2f* vertices, unsigned count) {

		assert (count > 2 && count <= MaxPolyVertex); // Только выпуклые полигоны
		count = std::min (count, MaxPolyVertex);


		int firstRight = 0;
		float highestXCoord = vertices[0].x;

		for (unsigned i = 1; i < count; ++i) {
			float X = vertices[i].x;
			if (X > highestXCoord) {
				highestXCoord = X;
				firstRight = i;
			}
			else if (X == highestXCoord) { // Eсли совпадают, берём верхнюю
				if (vertices[i].y < vertices[firstRight].y) {firstRight = i;}
			}
		}
		int hull[MaxPolyVertex];
		int outCount = 0;
		int indexHull = firstRight;

		while (true) {

			hull[outCount] = indexHull;
			// Поиск слелующей вершины по кругу (против часовой),
			// используя векторное произведение
			int nextHullIndex = 0;
			for (int idx = 1; idx < static_cast<int>(count); ++idx)
			{
				// Если 'следующий' совпадает с текущим, пропускаем
				if (nextHullIndex == indexHull) {
					nextHullIndex = idx;
					continue;
				}
				// http://www.oocities.org/pcgpe/math2d.html
				// Векторно перемножаем вектора, скользящие по граням
				// Если[e1 x e2]отрицательно,то idx --следующая вершина(против часовой)
				sf::Vector2f e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
				sf::Vector2f e2 = vertices[idx] - vertices[hull[outCount]];
				float crs = cross (e1, e2);
				if (crs < 0.f) {nextHullIndex = idx;}
				// Если[e1 x e2]равно нулю, значит они лежат на одной линии,
				// значит, мы согласны брать только самую дальнюю вершину
				if (crs == 0.f && lenSqrt (e2) > lenSqrt (e1)) {nextHullIndex = idx;}
			}
			++outCount;
			indexHull = nextHullIndex;

			if (nextHullIndex == firstRight) { // продолжать, пока не обогнём фигуру
				m_vertexCount = outCount;
				break;
			}
		}
		for(unsigned i = 0; i < m_vertexCount; ++i) {// копия вершин в вершины формы
			m_vertices[i] = vertices[hull[i]];
		}
		// Находим нормали граней
		for (unsigned it = 0, jt = 1; it < m_vertexCount; ++it, ++jt) {
			if (jt + 1 == m_vertexCount) {jt = 0;}
			sf::Vector2f face = m_vertices[jt] - m_vertices[it];
			assert (lenSqrt (face) > dontGiveAf * dontGiveAf); // нет граней с нулевым размером
			// Нормаль векторного произведения вектора и скаляра
			m_normals[it] = sf::Vector2f (face.y, -face.x);
			normalize (m_normals[it]);
		}
	}

	// The extreme point along a direction within a polygon
	sf::Vector2f GetSupport( const sf::Vector2f& dir ) {

		float bestProjection = -FLT_MAX;
		sf::Vector2f bestVertex;

		for (unsigned i = 0; i < m_vertexCount; ++i) {
			sf::Vector2f v = m_vertices[i];
			float projection = dot (v, dir);
			if (projection > bestProjection) {
				bestVertex = v;
				bestProjection = projection;
			}
		}

		return bestVertex;
	}
};


#endif

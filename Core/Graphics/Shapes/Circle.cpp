#include "Circle.hpp"
#include "../Shape.hpp"
#include "../../../Common/Util.hpp"

#include <Windows.h>

#include <objidl.h>
#include <gdiplus.h>
namespace gdi = Gdiplus;



namespace core
{
Circle::Circle(INT x, INT y, std::uint64_t radius)
	: position{ x, y }, radius{ radius } {}

Circle::Circle(POINT position, std::uint64_t radius)
	: position{ position }, radius{ radius } {}


Circle::~Circle() = default;


std::uint64_t Circle::getRadius() const
{
	return radius;
}

void Circle::setRadius(std::uint64_t radius)
{
	this->radius = radius;
}


void Circle::draw(HDC hdc) const
{
	gdi::Graphics graphics{ hdc };
	gdi::Pen pen{ gdi::Color::White };

	LONG longSize = static_cast<LONG>(radius);

	INT x{ static_cast<INT>(position.x - longSize) };
	INT y{ static_cast<INT>(position.y - longSize) };
	INT width{ static_cast<INT>(2 * longSize) };
	INT height{ static_cast<INT>(2 * longSize) };

	graphics.DrawEllipse(&pen, x, y, width, height);
}


bool Circle::intersect(POINT point) const
{
	double dradius{ static_cast<double>(radius) };
	return comm::distance(position, point) <= dradius;
}

POINT Circle::getPosition() const
{
	return position;
}

void Circle::setPosition(POINT position)
{
	this->position = position;
}
}

#include "Dot.hpp"

#include <Windows.h>

#include <objidl.h>
#include <gdiplus.h>
namespace gdi = Gdiplus;



namespace core
{
Dot::Dot(INT x, INT y) : position{ x, y } {}

Dot::Dot(POINT position) : position{ position } {}


void Dot::draw(HDC hdc) const
{
	gdi::Graphics graphics{ hdc };
	gdi::Pen pen{ gdi::Color::White };
	graphics.DrawRectangle(&pen, position.x, position.y, 0, 0);
}


bool Dot::intersect(POINT point) const
{
	return position.x == point.x && position.y == point.y;
}

POINT Dot::getPosition() const
{
	return position;
}

void Dot::setPosition(POINT position)
{
	this->position = position;
}
}

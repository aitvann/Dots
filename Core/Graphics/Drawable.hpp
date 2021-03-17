#pragma once

#include <Windows.h>

namespace core
{
/// An interface describing an object that can be drawn
class Drawable
{
public:
	Drawable();
	virtual ~Drawable();

	virtual void draw(HDC hdc) const = 0;
};
}

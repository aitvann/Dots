#pragma once

#include "Drawable.hpp"

#include <Windows.h>

#include <cstdint>



namespace core
{
/// An interface describing an object that can be drawn and has a position and size
class Shape : public Drawable
{
public:
	Shape();
	virtual ~Shape();

	virtual bool intersect(POINT point) const = 0;

	virtual POINT getPosition() const = 0;
	virtual void setPosition(POINT point) = 0;
};
}

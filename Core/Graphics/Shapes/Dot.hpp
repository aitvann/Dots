#pragma once

#include "../Shape.hpp"

#include <Windows.h>



namespace core
{
class Dot : public Shape
{
public:

	Dot(INT x = 0, INT y = 0);
	Dot(POINT position);

	void draw(HDC hdc) const override;

	bool intersect(POINT point) const override;
	POINT getPosition() const override;
	void setPosition(POINT position) override;


private:

	POINT position;
};
}

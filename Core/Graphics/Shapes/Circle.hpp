#pragma once

#include "../Shape.hpp"

#include <Windows.h>

#include <cstdint>



namespace core
{
class Circle : public Shape
{
public:

	Circle(INT x = 0, INT y = 0, std::uint64_t radius = 0);
	Circle(POINT position, std::uint64_t radius);

	virtual ~Circle();

	std::uint64_t getRadius() const;
	void setRadius(std::uint64_t radius);

	void draw(HDC hdc) const override;

	bool intersect(POINT point) const override;
	POINT getPosition() const override;
	void setPosition(POINT position) override;


private:

	POINT position;
	std::uint64_t radius;
};
}
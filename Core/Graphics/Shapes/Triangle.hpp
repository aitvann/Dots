#pragma once

#include "../Shape.hpp"
#include "Dot.hpp"

#include <objidl.h>
#include <gdiplus.h>
namespace gdi = Gdiplus;

#include <ranges>
#include <array>


namespace core
{
template <std::derived_from<Shape> S = Dot>
class Triangle : public Shape
{
public:

	using Vertix = S;
	using VertixRef = Vertix&;
	using VertixConstRef = Vertix const&;


	Triangle(Vertix a, Vertix b, Vertix c)
		: a{ a }, b{ b }, c{ c } {}

	virtual ~Triangle() = default;


	VertixConstRef getVertixA() const { return a; }
	VertixRef getVertixA() { return a; }

	VertixConstRef getVertixB() const { return b; }
	VertixRef getVertixB() { return b; }

	VertixConstRef getVertixC() const { return c; }
	VertixRef getVertixC() { return c; }


	LONG sign() const
	{
		auto [ax, ay] = a.getPosition();
		auto [bx, by] = b.getPosition();
		auto [cx, cy] = c.getPosition();

		return (ax - cx) * (by - cy) - (bx - cx) * (ay - cy);
	}


	void draw(HDC hdc) const override
	{
		a.draw(hdc);
		b.draw(hdc);
		c.draw(hdc);

		gdi::Graphics graphics{ hdc };
		gdi::Pen pen{ gdi::Color::White };

		auto [ax, ay] = a.getPosition();
		auto [bx, by] = b.getPosition();
		auto [cx, cy] = c.getPosition();

		std::array<gdi::Point, 3> points{ {
			gdi::Point{ ax, ay },
			gdi::Point{ bx, by },
			gdi::Point{ cx, cy }
		} };

		graphics.DrawPolygon(&pen, points.data(), 3);
	}


	bool intersect(POINT point) const override
	{
		Triangle triangle{ a.getPosition(), b.getPosition(), c.getPosition() };

		triangle.a.setPosition(point);
		LONG s1{ triangle.sign() };
		triangle.a.setPosition(a.getPosition());

		triangle.b.setPosition(point);
		LONG s2{ triangle.sign() };
		triangle.b.setPosition(b.getPosition());

		triangle.c.setPosition(point);
		LONG s3{ triangle.sign() };
		triangle.c.setPosition(c.getPosition());

		bool has_neg{ s1 < 0 || s2 < 0 || s3 < 0 };
		bool has_pos{ s1 > 0 || s2 > 0 || s3 > 0 };
		return !(has_neg && has_pos);
	}

	POINT getPosition() const override
	{
		auto [ax, ay] = a.getPosition();
		auto [bx, by] = b.getPosition();
		auto [cx, cy] = c.getPosition();

		auto x{ static_cast<double>(ax + bx + cx) / 3 };
		auto y{ static_cast<double>(ay + by + cy) / 3 };
		return POINT{ static_cast<LONG>(x), static_cast<LONG>(y) };
	}

	void setPosition(POINT position) override
	{
		POINT oldPos{ getPosition() };
		LONG dx{ oldPos.x - position.x };
		LONG dy{ oldPos.y - position.y };

		auto [ax, ay] = a.getPosition();
		a.setPosition(POINT{ ax - dx, ay - dy });

		auto [bx, by] = b.getPosition();
		b.setPosition(POINT{ bx - dx, by - dy });

		auto [cx, cy] = c.getPosition();
		c.setPosition(POINT{ cx - dx, cy - dy });
	}


private:

	Vertix a, b, c;
};
}
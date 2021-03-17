#pragma once

#include "../Input/InputReactor.hpp"
#include "../Graphics/Drawable.hpp"
#include "../Graphics/Shape.hpp"
#include "../../Common/Util.hpp"

#include <objidl.h>
#include <gdiplus.h>
namespace gdi = Gdiplus;

#include <vector>
#include <variant>
#include <limits>
#include <type_traits>
#include <concepts>

#include <algorithm>
namespace ranges = std::ranges;



namespace core
{
/// A container for shapes that allows the user to place and remove shapes
template <
	std::derived_from<Shape> S,
	comm::Converter<POINT, S> Conv = comm::DefaultConverter<POINT, S>
>
class PleaceableShapes : public Drawable, public InputReactor
{
public:

	using Shapes = std::vector<S>;

	PleaceableShapes(
		std::size_t minObjNum = 0,
		std::size_t maxObjNum = (std::numeric_limits<std::size_t>::max)(),
		Conv converter = comm::defaultConverter<POINT, S>
	) : minObjNum{ minObjNum }, maxObjNum{ maxObjNum }, converter{ converter } {}

	virtual ~PleaceableShapes() = default;

	constexpr std::size_t& getMinObjNum()
	{
		return minObjNum;
	}

	constexpr std::size_t& getMaxObjNum()
	{
		return maxObjNum;
	}

	constexpr Shapes& getShapes()
	{
		return shapes;
	}

	constexpr Conv const& getConverter() const
	{
		return converter;
	}

	void draw(HDC hdc) const override
	{
		for (auto const& shape : shapes)
			shape.draw(hdc);
	}


	template <typename = std::enable_if_t<std::is_base_of_v<InputReactor, S>>>
	bool delegateProcessInput(WindowInput const& windowInput)
	{
		auto pred = [&](S& shape) { return shape.processInput(windowInput); };
		return std::ranges::any_of(shapes, pred);
	}

	bool processInput(WindowInput const& windowInput) override
	{
		bool repaint{ false };

		std::visit(comm::match{
			[&](Input::DoubleClick input)
			{
				if (input.button != Mouse::LeftButton)
					return;

				repaint = true;
				auto pred = [&input](S const& s) { return s.intersect(input.position); };

				if (shapes.size() < maxObjNum && !ranges::any_of(shapes, pred))
					shapes.push_back(converter(input.position));
				else if (shapes.size() > minObjNum)
					shapes.erase(std::remove_if(shapes.begin(), shapes.end(), pred), shapes.end());
				else
					repaint = false;
			},
			[](auto) {}
		}, windowInput);

		if constexpr (std::is_base_of_v<InputReactor, S>)
			repaint |= delegateProcessInput(windowInput);

		return repaint;
	}


private:

	std::size_t minObjNum;
	std::size_t maxObjNum;
	Shapes shapes;

	Conv converter;
};
}

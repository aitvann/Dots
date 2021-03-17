#pragma once

#include "../Graphics/Shape.hpp"
#include "../Input/InputReactor.hpp"
#include "../../Common/Util.hpp"

#include <objidl.h>
#include <gdiplus.h>
namespace gdi = Gdiplus;

#include <type_traits>
#include <concepts>



namespace core
{
/// A shape decorator that allows the user to drag-and-drop it
template <std::derived_from<Shape> S>
class DragNDrop : public Shape, public InputReactor
{
public:

	DragNDrop(S shape) : m_shape{ shape }, m_dragged{ false } {}
	virtual ~DragNDrop() = default;


	S& getShape()
	{
		return m_shape;
	}

	bool isDragged() const
	{
		return m_dragged;
	}


	/******** Shape to m_shape delegation ********/
	void draw(HDC hdc) const override
	{
		m_shape.draw(hdc);
	}

	bool intersect(POINT point) const override
	{
		return m_shape.intersect(point);
	}

	POINT getPosition() const override
	{
		return m_shape.getPosition();
	}

	void setPosition(POINT position) override
	{
		m_shape.setPosition(position);
	}
	/******** End delegation ********/


	bool processInput(WindowInput const& windowInput) override
	{
		bool repaint{ false };

		std::visit(comm::match{
			[&](Input::MouseButtonPressedOnce input)
			{
				if (m_shape.intersect(input.position))
					m_dragged = true;
			},
			[&](Input::MouseMove input)
			{
				if (!m_dragged)
					return;

				m_shape.setPosition(input.position);
				repaint = true;
			},
			[&](Input::MouseButtonReleased input)
			{
				m_dragged = false;
			},
			[](auto) {}
			}, windowInput);

		if constexpr (std::is_base_of_v<InputReactor, S>)
			repaint |= delegateProcessInput(windowInput);

		return repaint;
	}
	

private:

	template <typename = std::enable_if_t<std::is_base_of_v<InputReactor, S>>>
	bool delegateProcessInput(WindowInput const& windowInput)
	{
		return m_shape.processInput(windowInput);
	}

	S m_shape;
	bool m_dragged;
};
}
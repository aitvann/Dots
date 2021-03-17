#include "InputListener.hpp"



namespace core
{
InputListener::InputListener()
	: m_oldMousePosition{ -1, -1 }, m_keyboard{}, m_mouse{} {}

InputListener::~InputListener() = default;

bool InputListener::updateInput(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		if (!m_keyboard.at(static_cast<std::size_t>(wParam)))
		{
			m_keyboard.at(static_cast<std::size_t>(wParam)) = true;
			return processInput({ Input::KeyPressedOnce{ static_cast<Keyboard>(wParam) } });
		}
		m_keyboard.at(static_cast<std::size_t>(wParam)) = true;
		return processInput({ Input::KeyPressed{ static_cast<Keyboard>(wParam) } });
	}

	case WM_KEYUP:
	{
		m_keyboard.at(static_cast<std::size_t>(wParam)) = false;
		return processInput({ Input::KeyReleased{ static_cast<Keyboard>(wParam) } });
	}

	case WM_LBUTTONDOWN:
	{
		m_mouse.at(static_cast<std::size_t>(Mouse::LeftButton)) = true;
		return processInput({ Input::MouseButtonPressedOnce{ Mouse::LeftButton, getMousePosition(lParam) } });
	}

	case WM_LBUTTONUP:
	{
		m_mouse.at(static_cast<std::size_t>(Mouse::LeftButton)) = false;
		return processInput({ Input::MouseButtonReleased{ Mouse::LeftButton, getMousePosition(lParam) } });
	}

	case WM_LBUTTONDBLCLK:
	{
		return processInput({ Input::DoubleClick{ Mouse::LeftButton, getMousePosition(lParam) } });
	}

	case WM_RBUTTONDOWN:
	{
		m_mouse.at(static_cast<std::size_t>(Mouse::RightButton)) = true;
		return processInput({ Input::MouseButtonPressedOnce{ Mouse::RightButton, getMousePosition(lParam) } });
	}

	case WM_RBUTTONUP:
	{
		m_mouse.at(static_cast<std::size_t>(Mouse::RightButton)) = false;
		return processInput({ Input::MouseButtonReleased{ Mouse::RightButton, getMousePosition(lParam) } });
	}

	case WM_RBUTTONDBLCLK:
	{
		return processInput({ Input::DoubleClick{ Mouse::RightButton, getMousePosition(lParam) } });
	}

	case WM_MBUTTONDOWN:
	{
		m_mouse.at(static_cast<std::size_t>(Mouse::MiddleButton)) = true;
		return processInput({ Input::MouseButtonPressedOnce{ Mouse::MiddleButton, getMousePosition(lParam) } });
	}

	case WM_MBUTTONUP:
	{
		m_mouse.at(static_cast<std::size_t>(Mouse::MiddleButton)) = false;
		return processInput({ Input::MouseButtonReleased{ Mouse::MiddleButton, getMousePosition(lParam) } });
	}

	case WM_MBUTTONDBLCLK:
	{
		return processInput({ Input::DoubleClick{ Mouse::MiddleButton, getMousePosition(lParam) } });
	}

	case WM_XBUTTONDOWN:
	{
		Mouse button{ HIWORD(wParam) == XBUTTON1 ?
			Mouse::XButton1 : Mouse::XButton2 };
		m_mouse.at(static_cast<std::size_t>(button)) = true;
		return processInput({ Input::MouseButtonPressedOnce{ button, getMousePosition(lParam) } });
	}

	case WM_XBUTTONUP:
	{
		Mouse button{ HIWORD(wParam) == XBUTTON1 ?
			Mouse::XButton1 : Mouse::XButton2 };
		m_mouse.at(static_cast<std::size_t>(button)) = false;
		return processInput({ Input::MouseButtonReleased{ button, getMousePosition(lParam) } });
	}

	case WM_XBUTTONDBLCLK:
	{
		return processInput({ Input::DoubleClick{ HIWORD(wParam) == XBUTTON1 ? Mouse::XButton1
			: Mouse::XButton2, getMousePosition(lParam) } });
	}

	case WM_MOUSEMOVE:
	{
		POINT position = getMousePosition(lParam);

		if (m_oldMousePosition.x == -1 && m_oldMousePosition.y == -1)
			m_oldMousePosition = position;

		bool repaint = processInput({ Input::MouseMove{ m_oldMousePosition, position } });
		m_oldMousePosition = position;
		return repaint;
	}

	case WM_MOUSEWHEEL:
	{
		return processInput({ Input::MouseWheel{ GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA,
			getMousePosition(lParam) } });
	}
	}

	return false;
}

bool InputListener::IsKeyPresssed(Keyboard key)
{
	return m_keyboard.at(static_cast<std::size_t>(key));
}

bool InputListener::IsMouseButtonPresssed(Mouse button)
{
	return m_mouse.at(static_cast<std::size_t>(button));
}

POINT InputListener::getMousePosition(LPARAM lParam)
{
	return { static_cast<std::uint16_t>(LOWORD(lParam)),
		static_cast<std::uint16_t>(HIWORD(lParam)) };
}
}

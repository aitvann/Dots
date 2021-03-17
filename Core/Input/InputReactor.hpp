#pragma once

#include "InputCodes.hpp"

#include <Windows.h>

#include <variant>



namespace core
{
namespace Input
{
	struct KeyPressed
	{
		Keyboard key;
	};

	struct KeyPressedOnce : KeyPressed {};

	struct KeyReleased : KeyPressedOnce {};

	struct MouseButtonPressedOnce
	{
		Mouse button;
		POINT position;
	};

	struct MouseButtonReleased : MouseButtonPressedOnce {};

	struct DoubleClick : MouseButtonPressedOnce {};

	struct MouseMove
	{
		POINT oldPosition;
		POINT position;
	};

	struct MouseWheel
	{
		std::int16_t wheel;
		POINT position;
	};
}

using WindowInput = std::variant<
	Input::KeyPressed,
	Input::KeyPressedOnce,
	Input::KeyReleased,
	Input::MouseButtonPressedOnce,
	Input::MouseButtonReleased,
	Input::DoubleClick,
	Input::MouseMove,
	Input::MouseWheel
>;

class InputReactor
{
public:

	InputReactor();
	~InputReactor();

	virtual bool processInput(WindowInput const& windowInput) = 0;
};
}
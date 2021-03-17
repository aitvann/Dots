#pragma once


#include "InputCodes.hpp"
#include "InputReactor.hpp"

#include <Windows.h>

#include <array>
#include <variant>



namespace core
{
/// An object that can listen to window input events and call a handler if needed
class InputListener : public InputReactor
{
public:

	InputListener();
	virtual ~InputListener();


protected:

	bool updateInput(UINT msg, WPARAM wParam, LPARAM lParam);

	bool IsKeyPresssed(Keyboard key);
	bool IsMouseButtonPresssed(Mouse button);


private:

	POINT getMousePosition(LPARAM lParam);

	std::array<bool, 256> m_keyboard;
	std::array<bool, 5> m_mouse;

	POINT m_oldMousePosition;
};
}

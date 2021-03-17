#pragma once

#include "../Core/Window/RenderWindow.hpp"
#include "../Core/Input/InputListener.hpp"
#include "../Core/Graphics/Shapes/Circle.hpp"
#include "../Core//UI/DragNDrop.hpp"
#include "../Core/UI/PleaceableShapes.hpp"



class MainWindow final : public core::RenderWindow, core::InputListener
{
public:

	MainWindow();
	~MainWindow();


private:

	using Vertix = core::DragNDrop<core::Circle>;

	static Vertix convert(POINT point);
	using Converter = decltype(&convert);


	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

	bool processInput(const core::WindowInput & windowInput) override;

	void begin() override;
	void end() override;

	void onPaint(HDC hdc);


	ULONG_PTR gdiplusToken;
	core::PleaceableShapes<Vertix, Converter> vertices;
};

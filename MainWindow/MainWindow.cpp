#include "MainWindow.hpp"
#include "../Core/UI/DragNDrop.hpp"
#include "../Core/Graphics/Shapes/Dot.hpp"
#include "../Core/Graphics/Shapes/Circle.hpp"
#include "../Core/Graphics/Shapes/Triangle.hpp"
#include "../Common/Util.hpp"

#include <Windows.h>

#include <objidl.h>
#include <gdiplus.h>
namespace gdi = Gdiplus;

#include <variant>
#include <iterator>

#include <ranges>
namespace views = std::views;
namespace ranges = std::ranges;



MainWindow::MainWindow() : gdiplusToken{ NULL }, vertices{ 0, 12, convert } {}

MainWindow::~MainWindow() = default;

// message handler
LRESULT MainWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// update input and repaint window if needed
	if (updateInput(msg, wParam, lParam))
		TRY(InvalidateRect(getHWnd(), nullptr, TRUE));

	if (msg == WM_CLOSE)
	{
		TRY(PostMessageW(hWnd, WM_QUIT, NULL, NULL));
	}
	else if (msg == WM_PAINT)
	{
		PAINTSTRUCT  ps;
		HDC hdc{ BeginPaint(getHWnd(), &ps) };
		onPaint(hdc);
		EndPaint(getHWnd(), &ps);
	}
	else
		return DefWindowProcW(hWnd, msg, wParam, lParam);

	return 0;
}

bool MainWindow::processInput(core::WindowInput const& windowInput)
{
	std::visit(comm::match{
		[&](core::Input::KeyPressedOnce input)
		{
			// close window by Esc button
			if (input.key == core::Keyboard::Easape)
				close();

			// switch fullscreen mod by F button
			if (input.key == core::Keyboard::F)
				switchFullscreenMode();
		},
		[](auto) {}
	}, windowInput);	

	// delegate input to vertices object
	return vertices.processInput(windowInput);
}

void MainWindow::begin()
{
	gdi::GdiplusStartupInput gdiplusStartupInput;
	gdi::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

void MainWindow::onPaint(HDC hdc)
{
	vertices.draw(hdc);

	gdi::Graphics graphics{ hdc };
	gdi::Pen pen{ gdi::Color::White };

	auto range = vertices.getShapes()
		| views::transform([](auto const& s) { return s.getPosition(); })
		| views::transform([](auto const& p) { return gdi::PointF(p.x, p.y); });
	std::vector<gdi::PointF> positions;
	positions.reserve(ranges::size(range));
	ranges::copy(range, std::back_inserter(positions));

	graphics.DrawClosedCurve(&pen, positions.data(), positions.size());
}

void MainWindow::end()
{
	gdi::GdiplusShutdown(gdiplusToken);
}


// converts POINT to Vertix
MainWindow::Vertix MainWindow::convert(POINT point)
{
	core::Circle circle{ point, 8 };
	return core::DragNDrop{ circle };
}

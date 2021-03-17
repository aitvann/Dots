#pragma comment (lib, "Gdiplus.lib")

#include "MainWindow/MainWindow.hpp"
#include "Common/Util.hpp"

#include <Windows.h>

#include <iostream>
#include <string>



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdLine, int show)
{
	core::RenderWindow::registerDefWC();

	MainWindow window;
	
	try
	{
		window.create(L"Dots", show, { 600, 600 });
		return static_cast<int>(window.run());
	}
	catch (std::exception& e)
	{
		std::string error{ comm::toString("Terminate by: ", e.what()) };

		MessageBoxExA(nullptr, error.c_str(), "Error", MB_OK,
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}

	core::RenderWindow::unregisterDefWC();

	return 0;
}
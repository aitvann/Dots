#include "Window.hpp"
#include "../../Common/Util.hpp"



namespace core
{
Window::Window(LPCWSTR className) : m_className{ className }, m_hWnd{ nullptr } {}

Window::~Window()
{
	close();
}

void Window::create(LPCWSTR windowName, int show, POINT size, POINT position, DWORD style, DWORD exStyle)
{
	if (m_hWnd)
		return;

	RECT rect{ position.x, position.y, size.x, size.y };
	TRY(AdjustWindowRect(&rect, style, false));

	TRY(m_hWnd = CreateWindowExW(exStyle, m_className, windowName, style,
		rect.left, rect.top, rect.right, rect.bottom, nullptr, nullptr,
		GetModuleHandleW(nullptr), this));

	TRY(ShowWindow(m_hWnd, show));
}

void Window::create(LPCWSTR windowName, int show, POINT size, DWORD style, DWORD exStyle)
{
	auto[width, height] = size;
	auto[x, y, resWidth, resHieght] = getMonitorRect(m_hWnd);
	POINT position{ (resWidth - width) / 2, (resHieght - height) / 2 };

	create(windowName, show, size, position, style, exStyle);
}

void Window::close()
{
	PostMessageW(m_hWnd, WM_CLOSE, NULL, NULL);
}

void * Window::getObjectFromHWnd(HWND hWnd)
{
	return reinterpret_cast<void*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
}

void Window::setObjectToHWnd(HWND hWnd, LPARAM lParam)
{
	LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lParam);
	TRY(SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams)));
}

RECT Window::getMonitorRect(HWND hWnd)
{
	HMONITOR hMon{ nullptr };
	TRY(hMon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST));

	MONITORINFO info{ 0 };
	info.cbSize = sizeof(MONITORINFO);
	TRY(GetMonitorInfoW(hMon, &info));
	
	return info.rcMonitor;
}

HWND Window::getHWnd() const
{
	return m_hWnd;
}

LPCWSTR Window::getClassName() const
{
	return m_className;
}
}

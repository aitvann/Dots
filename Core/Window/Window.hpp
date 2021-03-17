#pragma once

#include <Windows.h>



namespace core
{
class Window
{
public:
	explicit Window(LPCWSTR className);
	virtual ~Window();

	void create(LPCWSTR windowName, int show, POINT size, POINT position,
		DWORD style = WS_OVERLAPPEDWINDOW, DWORD exStyle = NULL);
	void create(LPCWSTR windowName, int show, POINT size,
		DWORD style = WS_OVERLAPPEDWINDOW, DWORD exStyle = NULL);
	void close();

	static void * getObjectFromHWnd(HWND hWnd);
	static void setObjectToHWnd(HWND hWnd, LPARAM lParam);

	static RECT getMonitorRect(HWND hWnd);

	HWND getHWnd() const;
	LPCWSTR getClassName() const;

protected:
	HWND m_hWnd;
	LPCWSTR m_className;
};
}

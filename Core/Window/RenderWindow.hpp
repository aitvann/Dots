#pragma once

#include "Window.hpp"



namespace core
{
class RenderWindow : public Window
{
public:
	explicit RenderWindow(LPCWSTR className = DEF_WC_CLASS_NAME);
	virtual ~RenderWindow();

	void createFullscreen(LPCWSTR windowName, INT show, POINT size, POINT position,
		DWORD style = WS_OVERLAPPEDWINDOW, DWORD exStyle = NULL);
	void createFullscreen(LPCWSTR windowName, INT show, POINT size,
		DWORD style = WS_OVERLAPPEDWINDOW, DWORD exStyle = NULL);

	void switchFullscreenMode(bool fullscreenMode);
	void switchFullscreenMode();

	static void registerDefWC();
	static void unregisterDefWC();

	bool isFullscreen() const;
	WPARAM run();

protected:
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK staticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static WNDCLASSEXW DEF_WC;

private:
	virtual void draw();
	virtual void tick();
	virtual void begin();
	virtual void end();

	struct
	{
		DWORD style;
		DWORD exStyle;
		RECT rect;
	} m_savedInfo;

	bool m_fullscreen;

	static constexpr LPCWSTR DEF_WC_CLASS_NAME = L"Default";
};
}

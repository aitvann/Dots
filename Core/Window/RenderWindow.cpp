#include "RenderWindow.hpp"
#include "../../Common/Util.hpp"



namespace core
{
RenderWindow::RenderWindow(LPCWSTR className) :
	Window{ className },
	m_savedInfo{ NULL, NULL, { 0, 0, 0, 0 } },
	m_fullscreen{ false }
{}

RenderWindow::~RenderWindow()
{
	close();
}

void RenderWindow::createFullscreen(LPCWSTR windowName, int show, POINT size, POINT position, DWORD style, DWORD exStyle)
{
	auto[mX, mY, mWidth, mHeight] = getMonitorRect(m_hWnd);
	create(windowName, show, { mWidth, mHeight }, { mX, mY }, WS_POPUP | WS_VISIBLE);

	auto[x, y] = position;
	auto[width, height] = size;
	m_savedInfo = { style, exStyle, { x, y, width + x, height + y } };

	m_fullscreen = true;
}

void RenderWindow::createFullscreen(LPCWSTR windowName, int show, POINT size, DWORD style, DWORD exStyle)
{
	auto[width, height] = size;
	auto[x, y, resWidth, resHieght] = getMonitorRect(m_hWnd);

	create(windowName, show, size, { (resWidth - width) / 2,
		(resHieght - height) / 2 }, style, exStyle);
}

void RenderWindow::switchFullscreenMode(bool fullscreenMode)
{
	if (fullscreenMode == m_fullscreen)
		return;

	if (fullscreenMode)
	{
		TRY(m_savedInfo.style = GetWindowLongW(m_hWnd, GWL_STYLE));
		TRY(m_savedInfo.exStyle = GetWindowLongW(m_hWnd, GWL_EXSTYLE));
		TRY(GetWindowRect(m_hWnd, &m_savedInfo.rect));

		TRY(SetWindowLongW(m_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE));

		auto[x, y, width, height] = getMonitorRect(m_hWnd);
		TRY(SetWindowPos(m_hWnd, nullptr, x, y, width, height, SWP_SHOWWINDOW));
	}
	else
	{
		TRY(SetWindowLongW(m_hWnd, GWL_STYLE, m_savedInfo.style));
		TRY(SetWindowLongW(m_hWnd, GWL_EXSTYLE, m_savedInfo.exStyle));

		auto[x, y, width, height] = m_savedInfo.rect;
		TRY(SetWindowPos(m_hWnd, nullptr, x, y, width - x, height - y, SWP_SHOWWINDOW));
	}

	m_fullscreen = fullscreenMode;
}

void RenderWindow::switchFullscreenMode()
{
	switchFullscreenMode(!m_fullscreen);
}

void RenderWindow::registerDefWC()
{
	TRY(RegisterClassExW(&DEF_WC));
}

void RenderWindow::unregisterDefWC()
{
	TRY(UnregisterClassW(DEF_WC.lpszClassName, GetModuleHandleW(nullptr)));
}

bool RenderWindow::isFullscreen() const
{
	return m_fullscreen;
}

WPARAM RenderWindow::run()
{
	if (!m_hWnd)
		return 0;

	begin();

	MSG msg{ 0 };
	while (true)
	{
		if (PeekMessageW(&msg, m_hWnd, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				TRY(DestroyWindow(m_hWnd));
				break;
			}

			TRY(TranslateMessage(&msg));
			TRY(DispatchMessageW(&msg));
		}
		else draw();

		tick();
	}

	end();

	return msg.wParam;
}

LRESULT RenderWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CLOSE)
	{
		TRY(PostMessageW(hWnd, WM_QUIT, NULL, NULL));
	}
	else
		return DefWindowProcW(hWnd, msg, wParam, lParam);

	return 0;
}

WNDCLASSEXW RenderWindow::DEF_WC{
	sizeof(WNDCLASSEXW),
	CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
	staticWndProc,
	0, 0,
	GetModuleHandleW(nullptr),
	nullptr,
	LoadCursorW(nullptr, IDC_ARROW),
	(HBRUSH)(COLOR_BACKGROUND + 1),
	nullptr,
	DEF_WC_CLASS_NAME,
	nullptr
};

LRESULT CALLBACK RenderWindow::staticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
		Window::setObjectToHWnd(hWnd, lParam);
	RenderWindow * pThis{ reinterpret_cast<RenderWindow*>(Window::getObjectFromHWnd(hWnd)) };

	if (pThis)
		return pThis->WndProc(hWnd, msg, wParam, lParam);
	else
		return DefWindowProcW(hWnd, msg, wParam, lParam);
}

void RenderWindow::draw() {}

void RenderWindow::tick() {}

void RenderWindow::begin() {}

void RenderWindow::end() {}
}

#pragma once

namespace Demx
{
	class Engine;

	class Window
	{
		friend Engine;

		// handle of instance
		HINSTANCE	hInstance;

		// device context
		HDC			hDC;

		// render context
		HGLRC		hRC;

		// window handle
		HWND		hWnd;

	public:
		Window( HINSTANCE hInstance );
		~Window(void);

		HINSTANCE getInstanceHandle();
		HDC getDeviceContext();
		HGLRC getRenderContext();
		HWND getWindowHandle();
	};
}
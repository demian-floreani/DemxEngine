#include "stdafx.h"
#include "Window.h"

Demx::Window::Window( HINSTANCE hInstance )
{
	this->hInstance = hInstance;
}

Demx::Window::~Window(void)
{

}

HINSTANCE Demx::Window::getInstanceHandle()
{
	return hInstance;
}

HDC Demx::Window::getDeviceContext()
{
	return hDC;
}

HGLRC Demx::Window::getRenderContext()
{
	return hRC;
}

HWND Demx::Window::getWindowHandle()
{
	return hWnd;
}
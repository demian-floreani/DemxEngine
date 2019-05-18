#include "stdafx.h"

#include "Engine.h"
#include "Renderer.h"

GL_HRESULT Demx::Engine::createWindow( Demx::Window* window, LPCWSTR title, long width, long height, WNDPROC lpfnWndProc )
{
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= lpfnWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= window->hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= L"OpenGL";
	wc.hIconSm = NULL;

	// registers a window class for use in calls to CreateWindow
	if( !RegisterClassEx( &wc ) )
	{
		return GL_FAIL;
	}
	
	RECT rect;
	rect.left = 0;
	rect.right = width;
	rect.top = 0;
	rect.bottom = height;

	int windowWidth = (int) (rect.right - rect.left);
	int windowHeight = (int) (rect.bottom - rect.top);

	// calculates the required size of the window rectangle
	AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);

	// creates a window with an extended window style
	window->hWnd = CreateWindowEx(	dwExStyle,
									L"OpenGL",
									title,
									dwStyle | WS_CLIPSIBLINGS |	WS_CLIPCHILDREN,				
									0, 
									0,
									windowWidth,
									windowHeight,
									NULL,
									NULL,
									window->hInstance,
									NULL
								);

	// retrieves a handle to a device context (DC) of a specified window
	if ( !( window->hDC = GetDC( window->hWnd ) ) )
	{
		return GL_FAIL;
	}

	PIXELFORMATDESCRIPTOR pfd;

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	GLuint pixelFormat;			
	if ( !( pixelFormat = ChoosePixelFormat( window->hDC, &pfd ) ) )
	{
		return GL_FAIL;
	}

	if( !SetPixelFormat( window->hDC, pixelFormat, &pfd) )
	{
		return GL_FAIL;
	}

	// creates a new OpenGL rendering context
	if ( !( window->hRC = wglCreateContext( window->hDC ) ) )
	{
		return GL_FAIL;
	}

	// makes a specified OpenGL rendering context the current rendering context
	if( !wglMakeCurrent( window->hDC, window->hRC ) )
	{
		return GL_FAIL;
	}

	ShowWindow( window->hWnd, SW_SHOW );
	SetForegroundWindow( window->hWnd );	
	SetFocus( window->hWnd );

	this->renderer->resize( width, height );

	return GL_OK;
}

GL_HRESULT Demx::Engine::closeWindow( Demx::Window* window )
{
	if( window->hRC )
	{
		// If hglrc is NULL, the function makes the calling thread's current rendering context no longer current
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( window->hRC );
	}

	if( window->hDC )
	{
		// releases a device context
		ReleaseDC( window->hWnd, window->hDC );
	}

	if( window->hWnd )
	{
		// destroys the specified window
		DestroyWindow( window->hWnd );
	}

	if( window->hInstance )
	{
		// unregisters a window class
		UnregisterClass( L"OpenGL", window->hInstance );
	}

	return GL_OK;
}
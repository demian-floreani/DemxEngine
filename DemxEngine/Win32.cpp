#include "stdafx.h"

#include <io.h>
#include <fcntl.h>

#include "Engine.h"

#include "MainScene.h"
#include "Scene2.h"
#include "ParticleScene.h"
#include "TestScene.h"

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

Demx::Engine engine;

LRESULT CALLBACK WndProc(	HWND		hWnd,
							UINT		uMsg,
							WPARAM		wParam,
							LPARAM		lParam)
{
	engine.processInput( uMsg, wParam, lParam );
	
	switch (uMsg)
	{
		// exit program
		case WM_CHAR:
		{
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				return 0;
			}
		}break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

int CALLBACK WinMain(	HINSTANCE	hInstance,
						HINSTANCE	hPrevInstance,
						LPSTR		lpCmdLine,
						int			nCmdShow)
{
	Demx::Window* window = new Demx::Window( hInstance );

	MSG	msg;
	bool running = true;

	// Setup the console for debugging
	AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;

	SetWindowPos( GetConsoleWindow(), HWND_NOTOPMOST, 1050, 0, 0, 0, SWP_NOSIZE );

	ShowCursor( FALSE );

	if( engine.createWindow( window, L"DemxEngine", 1024, 768, WndProc ) == GL_FAIL )
	{
		engine.closeWindow( window );
		delete window;
		return 0;
	}

	if( engine.init( new TestScene() ) != GL_OK )
	{
		return 0;
	}

	while( running )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
			{
				running = false;
			}
			else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			if( engine.update() != GL_OK )
			{
				running = false;
			}

			if ( engine.render() != GL_OK )
			{
				running = false;
			}
			else
			{
				SwapBuffers( window->getDeviceContext() );
			}
		}
	}

	engine.close();

	engine.closeWindow( window );
	delete window;

	return (msg.wParam);
}

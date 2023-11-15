#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "viewport.h"
#include <iostream>
#include <windows.h>
#include <tchar.h>

#define TO_FILE "TO_FILE"

#ifndef TO_FILE
#define render renderToFile
#else
#define render renderToWindow
#endif

constexpr int WINDOW_WIDTH = 1366;
constexpr float ASPECT_RATIO = 16.0 / 9.0;

void renderToFile(
	Viewport vp,
	HDC hdc = 0
) {

	// Header of picture file
	std::cout << "P3" << std::endl;
	std::cout << vp.imageWidth() << ' ' << vp.imageHeight() << std::endl;
	std::cout << 255 << std::endl;

	for (int j = 0; j < vp.imageHeight(); j++) {
		std::clog << "\r Scanlines Remaining: " << (vp.imageHeight() - j) << ' ' << std::flush;
		for (int i = 0; i < vp.imageWidth(); i++) {
			/*
				the center of the pixel is calculated by multiplying our deltas for x and y
				by our offsets and adding to the center of the first pixel in the grid
			*/
			Vec3 pixelCenter = vp.firstPixelLocation() + (vp.pixelDeltaU() * i) + (vp.pixelDeltaV() * j);
			/*
				our ray's direction is determined by subtracting our camera center, or eye
				from the location of the pixel we are currently painting
			*/
			Vec3 rayDirection = pixelCenter - vp.cameraCenter();
			/*
				we create our ray with the origin being camera center, or eye, and the
				direction being what we just calculated above
			*/
			Ray r(vp.cameraCenter(), rayDirection);
			/*
				we create a color vector for the ray we just created above
			*/
			Color pixelColor = r.color();
			/*
				we write the color to our image file for displaying
			*/
			pixelColor.writeColor(std::cout);
		}
	}

	std::clog << "\r Done." << std::endl;
}

void renderToWindow(
	Viewport vp,
	HDC hdc
) {
	for (int j = 0; j < vp.imageHeight(); j++) {
		for (int i = 0; i < vp.imageWidth(); i++) {
			/*
				the center of the pixel is calculated by multiplying our deltas for x and y
				by our offsets and adding to the center of the first pixel in the grid
			*/
			Vec3 pixelCenter = vp.firstPixelLocation() + (vp.pixelDeltaU() * i) + (vp.pixelDeltaV() * j);
			/*
				our ray's direction is determined by subtracting our camera center, or eye
				from the location of the pixel we are currently painting
			*/
			Vec3 rayDirection = pixelCenter - vp.cameraCenter();
			/*
				we create our ray with the origin being camera center, or eye, and the
				direction being what we just calculated above
			*/
			Ray r(vp.cameraCenter(), rayDirection);
			/*
				we create a color vector for the ray we just created above
			*/
			Color pixelColor = r.color();
			/*
				we write the color to our image file for displaying
			*/
			COLORREF ref = RGB(pixelColor.r(), pixelColor.g(), pixelColor.b());
			SetPixel(hdc, i, j, ref);
		}
	}
}


LRESULT CALLBACK WindowProc(
	_In_ HWND hWnd,
	_In_ UINT message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	HDC hdc;
	Viewport vp;

	switch (message) {
	case WM_PAINT:
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		vp = Viewport(WINDOW_WIDTH);
		render(vp, hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {

	static TCHAR szWindowClass[] = _T("Rendy");
	static TCHAR szTitle[] = _T("Rendy");

	// WNDCLASSEX is a Win32 class that contains window information
	// MSDN doc: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	WNDCLASSEX wcex;
	// cbSize is the memory size of the class struct in bytes
	wcex.cbSize = sizeof(WNDCLASSEX);
	// Can be any combination of Window class styles
	// See: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-class-styles
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	// A pointer to the Window Procedure defined above
	wcex.lpfnWndProc = WindowProc;
	// Number of extra bytes to allocate beyond the class struct size
	// The bytes are initialized to 0
	wcex.cbClsExtra = 0;
	// Number of extra bytes to allocate following the Window instance
	// The bytes are initialized to 0
	wcex.cbWndExtra = 0;
	// Handle to the instance that contains the window procedure for the class
	wcex.hInstance = hInstance;
	// A handle to the class icon
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	// A handle to the class cursor resource
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	// A hangle to the class background brush, look into HBRUSH types
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	// Pointer to a character string that sets the resource name of the class menu
	wcex.lpszMenuName = NULL;
	// A pointer to a string that sets the window class name
	wcex.lpszClassName = szWindowClass;
	// A handle to a small icon that is associated with the window class
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	// If we can't successfully register the window class, kill Rendy
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Rendy"),
			NULL);

		return 1;
	}

	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,									// An optional extended window style
		szWindowClass,											// The name of the application
		szTitle,												// The text that appears in the title bar
		WS_OVERLAPPEDWINDOW,									// The type of window to create
		CW_USEDEFAULT,											// Inital position x
		CW_USEDEFAULT,											// Initial position y
		WINDOW_WIDTH,											// The width of the window
		max(static_cast<int>(WINDOW_WIDTH / ASPECT_RATIO), 1),	// The height of the window
		NULL,													// The parent of this window
		NULL,													// This application does not have a menu bar
		hInstance,												// The first parameter from WinMain
		NULL													// Not used in this application
	);

	// If we can't successfully create the window and get the handle, kill Rendy
	if (!hWnd) {
		MessageBox(NULL,
			_T("Call to CreateWindowEx failed!"),
			_T("Rendy"),
			NULL);

		return 1;
	}

	// Sets the window's show state
	// See: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	ShowWindow(
		hWnd,		// The value returned from CreateWindow; handle to the window
		nCmdShow	// From WinMain parameters
	);

	// Updates the client area of the window by sending a WM_PAINT message to the window
	// See: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-updatewindow
	UpdateWindow(hWnd);

	// Message loop
	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

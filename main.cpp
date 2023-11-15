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

	static TCHAR szWindowClass[] = _T("RayTracer");
	static TCHAR szTitle[] = _T("Rendy");

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Rendy"),
			NULL);

		return 1;
	}

	// The parameters to CreateWindowEx explained:
	// WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		max(static_cast<int>(WINDOW_WIDTH / ASPECT_RATIO), 1),
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd) {
		MessageBox(NULL,
			_T("Call to CreateWindowEx failed!"),
			_T("Rendy"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// Message loop
	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

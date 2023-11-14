#include "vec3.h"
#include "ray.h"
#include "color.h"
#include <iostream>
#include <windows.h>
#include <tchar.h>

#define TO_FILE "TO_FILE"

#ifndef TO_FILE
#define render renderToFile
#else
#define render renderToWindow
#endif

void renderToFile(
	int IMAGE_WIDTH,
	int IMAGE_HEIGHT,
	Vec3 firstPixelLocation,
	Vec3 CAMERA_CENTER,
	Vec3 pixelDeltaU,
	Vec3 pixelDeltaV,
	HDC hdc = 0
) {

	// Header of picture file
	std::cout << "P3" << std::endl;
	std::cout << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << std::endl;
	std::cout << 255 << std::endl;

	for (int j = 0; j < IMAGE_HEIGHT; j++) {
		std::clog << "\r Scanlines Remaining: " << (IMAGE_HEIGHT - j) << ' ' << std::flush;
		for (int i = 0; i < IMAGE_WIDTH; i++) {
			/*
				the center of the pixel is calculated by multiplying our deltas for x and y
				by our offsets and adding to the center of the first pixel in the grid
			*/
			Vec3 pixelCenter = firstPixelLocation + (pixelDeltaU * i) + (pixelDeltaV * j);
			/*
				our ray's direction is determined by subtracting our camera center, or eye
				from the location of the pixel we are currently painting
			*/
			Vec3 rayDirection = pixelCenter - CAMERA_CENTER;
			/*
				we create our ray with the origin being camera center, or eye, and the
				direction being what we just calculated above
			*/
			Ray r(CAMERA_CENTER, rayDirection);
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
	int IMAGE_WIDTH,
	int IMAGE_HEIGHT,
	Vec3 firstPixelLocation,
	Vec3 CAMERA_CENTER,
	Vec3 pixelDeltaU,
	Vec3 pixelDeltaV,
	HDC hdc
) {
	for (int j = 0; j < IMAGE_HEIGHT; j++) {
		for (int i = 0; i < IMAGE_WIDTH; i++) {
			/*
				the center of the pixel is calculated by multiplying our deltas for x and y
				by our offsets and adding to the center of the first pixel in the grid
			*/
			Vec3 pixelCenter = firstPixelLocation + (pixelDeltaU * i) + (pixelDeltaV * j);
			/*
				our ray's direction is determined by subtracting our camera center, or eye
				from the location of the pixel we are currently painting
			*/
			Vec3 rayDirection = pixelCenter - CAMERA_CENTER;
			/*
				we create our ray with the origin being camera center, or eye, and the
				direction being what we just calculated above
			*/
			Ray r(CAMERA_CENTER, rayDirection);
			/*
				we create a color vector for the ray we just created above
			*/
			Color pixelColor = r.color();
			/*
				we write the color to our window for displaying
			*/
			COLORREF ref = RGB(pixelColor.r(), pixelColor.g(), pixelColor.b());
			SetPixel(hdc, i, j, ref);
		}
	}
}


int init(HDC hdc = NULL) {
	
	/*
		A note on the aspect ratio mentioned below: it is easier to do the math to calculate
		the height of an image given the width and aspect ratio, rather than statically definig
		it. That way, we can simply change the width of the image and have the proportions of
		the render be preserved.
	*/

	/*
		A note on the viewport mentioned below: the viewport is a virtual concept. In our case,
		the viewport is a virtual rectangle that holds the grid of pixels we are painting. Square
		pixels are standard. If the spacing between pixels is the same horizontally and vertically,
		then the aspect ratio of the viewport matches the aspect ratio of the render, which is the ratio
		we defined above.

		We do not use the calculated aspect ratio to calculate viewport width due to the
		integer nature of the render height. Instead we get the real ratio by dividing render width
		by render height. Not doing this could result in clipping.
	*/

	// Canvas Constants
	const int IMAGE_WIDTH = 1920;
	const float ASPECT_RATIO = 16.0 / 9.0;

	// Image Height Calculation
	/*
		The height of the render must be at least 1 (meaning at least 1 pixel high)
	*/
	const int IMAGE_HEIGHT = max(static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO), 1);

	// Camera Calculations
	/*
		The focal length is the unit of distance of the vector from the camera
		center point and the viewport
	*/
	const float FOCAL_LENGTH = 1.0;
	/*
		This height of 2.0 is completely arbitrary

		A viewport height of less than 1 is allowed
	*/
	const float VIEWPORT_HEIGHT = 2.0;
	/*
		We calculate the viewport width based on the arbitrary view port height and the real
		aspect ratio of the render calculated by dividing render width by render height
	*/
	const float VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (static_cast<float>(IMAGE_WIDTH) / IMAGE_HEIGHT);
	/*
		We arbitrarily set our camera to be centered on point(0, 0, 0)

		You can also think of this point as the "eye"
	*/
	Vec3 CAMERA_CENTER = Vec3();

	// Calculate the vectors across and down the viewport edges
	/*
		viewportU represents the size of the viewport in the x direction
	*/
	Vec3 viewportU = Vec3(VIEWPORT_WIDTH, 0, 0);
	/*
		viewportV represents the size of the viewport in the y direction
		
		viewportV uses the negative of the viewport height because
		when we traverse the grid of pixels, we start at (0,0) and
		traverse downward
	*/
	Vec3 viewportV = Vec3(0, -VIEWPORT_HEIGHT, 0);

	// Calculate the delta vectors from pixel to pixel
	/* 
		the delta between pixels in the x direction is
		calculated by dividing the x direction vector by
		the absolute width of the image
	*/
	Vec3 pixelDeltaU = viewportU / IMAGE_WIDTH;
	/*
		the delta between pixels in the y direction is
		calculated by dividing the y direction vector by
		the absolute height of the image
	*/
	Vec3 pixelDeltaV = viewportV / IMAGE_HEIGHT;

	// Calculate the location of the upper left pixel
	/*
		to calculate the upper left of our viewport, we start at our camera center, 
		or eye, and we subtract our focal length from the z, we subtract half of our 
		viewportU from the x, and subtract half of our viewportV from the y

		the reason we halve our x and y vectors is because the pixel grid will be inset
		from the edges of the viewport by half the delta between pixels, so that the 
		viewport area will have perfectly even width x height regions
	*/
	Vec3 viewportUpperLeft = CAMERA_CENTER - Vec3(0, 0, FOCAL_LENGTH) - viewportU / 2 - viewportV / 2;
	/*
		once we have the upper left bound of our viewport edge, the center of the first pixel of our grid that we will paint
		is located half of the x delta and half of the y delta from our left upper bound of our viewport
	*/
	Vec3 firstPixelLocation = viewportUpperLeft + ((pixelDeltaU + pixelDeltaV) * 0.5);

	render(
		IMAGE_WIDTH,
		IMAGE_HEIGHT,
		firstPixelLocation,
		CAMERA_CENTER,
		pixelDeltaU,
		pixelDeltaV,
		hdc
	);

	return 0;
}


LRESULT CALLBACK WindowProc(
	_In_ HWND hWnd,
	_In_ UINT message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	HDC hdc;

	switch (message) {
	case WM_PAINT:
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		init(hdc);
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

	// Canvas Constants
	const int IMAGE_WIDTH = 1920;
	const float ASPECT_RATIO = 16.0 / 9.0;

	// Image Height Calculation
	/*
		The height of the render must be at least 1 (meaning at least 1 pixel high)
	*/
	const int IMAGE_HEIGHT = max(static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO), 1);

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
		IMAGE_WIDTH, IMAGE_HEIGHT,
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
}

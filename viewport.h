#pragma once
#include "vec3.h"

/*
	A note on the viewport mentioned below: the viewport is a virtual concept. In our case,
	the viewport is a virtual rectangle that holds the grid of pixels we are painting. Square
	pixels are standard. If the spacing between pixels is the same horizontally and vertically,
	then the aspect ratio of the viewport matches the aspect ratio of the render, which is the ratio
	we defined above..
*/
class Viewport {

private:
	Vec3 cC;
	Vec3 pDU;
	Vec3 pDV;
	Vec3 fPL;
	int iH;
	int iW;
public:
	/*
		A note on the aspect ratio mentioned below: it is easier to do the math to calculate
		the height of an image given the width and aspect ratio, rather than statically definig
		it. That way, we can simply change the width of the image and have the proportions of
		the render be preserved.

		The focal length is the unit of distance of the vector from the camera
		center point and the viewport

		This viewport height of 2.0 is completely arbitrary. A viewport height of less than 1 is allowed

		We arbitrarily set our camera to be default centered on point(0, 0, 0).
		You can also think of this point as the "eye"
	*/
	Viewport(
		int imageWidth = 800,
		float aspectRatio = 16.0 / 9.0,
		float focalLength = 1.0,
		float viewportHeight = 2.0,
		Vec3 cameraCenter = Vec3(0, 0, 0)
	) {

		// Image Height Calculation
		/*
			The height of the render must be at least 1 (meaning at least 1 pixel high)
		*/
		const int imageHeight = std::max(static_cast<int>(imageWidth / aspectRatio), 1);

		// Viewport dimension calculation
		/*
			We calculate the viewport width based on the arbitrary view port height and the real
			aspect ratio of the render calculated by dividing render width by render height
		*/
		const float viewportWidth = viewportHeight * (static_cast<float>(imageWidth) / imageHeight);

		// Calculate the vectors across and down the viewport edges
		/*
			viewportU represents the size of the viewport in the x direction
		*/
		Vec3 viewportU = Vec3(viewportWidth, 0, 0);
		/*
			viewportV represents the size of the viewport in the y direction

			viewportV uses the negative of the viewport height because
			when we traverse the grid of pixels, we start at (0,0) and
			traverse downward
		*/
		Vec3 viewportV = Vec3(0, -viewportHeight, 0);

		// Calculate the delta vectors from pixel to pixel
		/*
			the delta between pixels in the x direction is
			calculated by dividing the x direction vector by
			the absolute width of the image
		*/
		Vec3 pixelDeltaU = viewportU / imageWidth;
		/*
			the delta between pixels in the y direction is
			calculated by dividing the y direction vector by
			the absolute height of the image
		*/
		Vec3 pixelDeltaV = viewportV / imageHeight;

		// Calculate the location of the upper left pixel
		/*
			to calculate the upper left of our viewport, we start at our camera center,
			or eye, and we subtract our focal length from the z, we subtract half of our
			viewportU from the x, and subtract half of our viewportV from the y

			the reason we halve our x and y vectors is because the pixel grid will be inset
			from the edges of the viewport by half the delta between pixels, so that the
			viewport area will have perfectly even width x height regions
		*/
		Vec3 viewportUpperLeft = cameraCenter - Vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
		/*
			once we have the upper left bound of our viewport edge, the center of the first pixel of our grid that we will paint
			is located half of the x delta and half of the y delta from our left upper bound of our viewport
		*/
		Vec3 firstPixelLocation = viewportUpperLeft + ((pixelDeltaU + pixelDeltaV) * 0.5);

		this->cC = cameraCenter;
		this->pDU = pixelDeltaU;
		this->pDV = pixelDeltaV;
		this->fPL = firstPixelLocation;
		this->iW = imageWidth;
		this->iH = imageHeight;
	}

	// Getters
	const Vec3 cameraCenter() const { return this->cC; }
	const Vec3 pixelDeltaU() const { return this->pDU; }
	const Vec3 pixelDeltaV() const { return this->pDV; }
	const Vec3 firstPixelLocation() const { return this->fPL; }
	const int imageWidth() const{ return this->iW; }
	const int imageHeight() const { return this->iH; }
};
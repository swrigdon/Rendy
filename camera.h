#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "rendyUtils.h"
#include "pixel.h"
#include "viewport.h"
#include <windows.h>
#include <tchar.h>

class Camera {
	private:
		
		Viewport _viewport;
		Vec3 _cameraCenter;

	public:
		Camera() {
			_cameraCenter = Vec3(0, 0, 0);
			_viewport = Viewport();
		}

		Camera(int windowWidth, float aspectRatio) {
			_cameraCenter = Vec3(0, 0, 0);
			_viewport = Viewport(windowWidth, aspectRatio, _cameraCenter);
		}

		Camera(int windowWidth, float aspectRatio, Vec3 cameraCenter) {
			_cameraCenter = cameraCenter;
			_viewport = Viewport(windowWidth, aspectRatio, _cameraCenter);
		}

		// Getters
		const int imageWidth() const { return _viewport.imageWidth(); }
		const int imageHeight() const { return _viewport.imageHeight(); }
		const Vec3 cameraCenter() const { return _cameraCenter; }

		void render(
			const int aliasSamples,
			const int maxDepth,
			Surface& sceneObjects,
			HDC hdc
		) {
			for (int j = 0; j < _viewport.imageHeight(); j++) {
				for (int i = 0; i < _viewport.imageWidth(); i++) {
					/*
						the center of the pixel is calculated by multiplying our deltas for x and y
						by our offsets and adding to the center of the first pixel in the grid
					*/
					Vec3 pixelCenter = _viewport.firstPixelLocation() + (_viewport.pixelDeltaU() * i) + (_viewport.pixelDeltaV() * j);
					/*
						our ray's direction is determined by subtracting our camera center, or eye
						from the location of the pixel we are currently painting
					*/
					Vec3 rayDirection = pixelCenter - _cameraCenter;
					/*
						do our AA sampling passes
					*/
					Vec3 aaColor = Vec3(0, 0, 0);
					for (int sample = 0; sample < aliasSamples; sample++) {
						/*
						we create our ray with the origin being camera center, or eye, and the
						direction being what we just calculated above
						*/
						Ray r = getRay(pixelCenter);
						Pixel pixel = Pixel(maxDepth, sceneObjects, r, i, j);
						aaColor += pixel.getColorVector();
					}
					aaColor = antiAlias(aliasSamples, aaColor);
					/*
						Perform the coloring calculation for the pixel in the Color constructor
						and get the Window COLORREF using the getColorRef method
					*/
					COLORREF ref = RGB(aaColor.x(), aaColor.y(), aaColor.z());
					/*
						Set the color of the pixel in the window using the COLORREF
					*/
					SetPixel(hdc, i, j, ref);
				}
			}
		}

		Ray getRay(Vec3 pixelCenter) const {
			Vec3 pixelSample = getSampleSquare() + pixelCenter;
			return Ray(_cameraCenter, pixelSample - _cameraCenter);
		}

		Vec3 getSampleSquare() const {
			float px = -0.5 + random_float();
			float py = -0.5 + random_float();
			return (_viewport.pixelDeltaU() * px) + (_viewport.pixelDeltaV() * py);
		}

		Vec3 antiAlias(int aliasSamples, Vec3 color) {
			return color * (1.0 / aliasSamples);
		}
};

#endif

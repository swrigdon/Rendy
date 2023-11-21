#pragma once
#ifndef  INTERVAL_H
#define INTERVAL_H

#include "rendyUtils.h"

class Interval {
	public:
		float min, max;

		Interval() : min(+infinity), max(-infinity) {}
		Interval(float _min, float _max) : min(_min), max(_max) {}

		bool contains(float x) const {
			return min <= x && x <= max;
		}

		bool surrounds(float x) const {
			return min < x && x < max;
		}

		static const Interval empty, universe;
};

const static const Interval empty(+infinity, -infinity);
const static const Interval universe(-infinity, +infinity);

#endif // ! INTERVAL_H

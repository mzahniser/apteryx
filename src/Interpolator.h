/* Interpolator.h
Michael Zahniser, 10/25/06

Header file for the Interpolator class, which performs fast bilinear interpolation.
*/

#ifndef __MZ_INTERPOLATOR_H__
#define __MZ_INTERPOLATOR_H__


#include "Image.h"



class Interpolator {
public:
	void operator()(const GreyImage &source, GreyImage &destination);


private:
	class SlopeData {
	public:
		long slopeH;
		long nextSlopeH;
		long deltaSlopeH;
		long slopeV;
		long firstValue;
		long columns;
		long fraction;
	};

	Image<SlopeData> slopeDataArray;

	Image<long> slopeH;
	Image<long> nextSlopeH;
	Image<long> deltaSlopeH;
	Image<long> slopeV;
	Image<long> firstValue;
};

#endif
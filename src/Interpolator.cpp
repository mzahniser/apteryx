/* Interpolator.cp
Michael Zahniser, 10/25/06

Function definitions for the Interpolator class.
*/

#include "Interpolator.h"



void Interpolator::operator()(const GreyImage &source, GreyImage &destination)
{
	int sourceWidth = source.width - 1;
	int sourceHeight = source.height - 1;

	slopeDataArray.Size(sourceWidth, 1);

	// Each pixel in the source image occupies a certain number of columns of the destination image,
	// and its first pixel in the destination image is some fractional step past the start of the source pixel.
	// Precalculate all column widths and fractional increments.
	SlopeData *slopeData = slopeDataArray.data;
	SlopeData *slopeDataEnd = slopeData + sourceWidth;

	int fraction = 0;
	int columns = 0;
	slopeData->fraction = 0;

	while(true)
	{
		++columns;
		fraction += sourceWidth;

		if(fraction >= destination.width)
		{
			slopeData->columns = columns;

			++slopeData;
			if(slopeData == slopeDataEnd) break;

			columns = 0;

			fraction -= destination.width;
			slopeData->fraction = fraction;
		}
	}

	// Calculate the first set of horizontal and vertical slopes.
	unsigned char *firstData = source.data;
	unsigned char *secondData = firstData + source.width;

	long firstPixel = (*firstData << 16);
	long secondPixel = (*secondData << 16);

	for(slopeData = slopeDataArray.data; slopeData != slopeDataEnd; ++slopeData)
	{
		++firstData;
		++secondData;
		
		long newFirstPixel = (*firstData << 16);
		long newSecondPixel = (*secondData << 16);

		slopeData->slopeH = (sourceWidth * (newFirstPixel - firstPixel)) / destination.width;
		slopeData->nextSlopeH = (sourceWidth * (newSecondPixel - secondPixel)) / destination.width;
		slopeData->deltaSlopeH = (sourceHeight * (slopeData->nextSlopeH - slopeData->slopeH)) / destination.height;
		slopeData->slopeV = (sourceHeight * (secondPixel - firstPixel)) / destination.height;
		slopeData->firstValue = firstPixel + (slopeData->slopeH * slopeData->fraction) / destination.width;

		firstPixel = newFirstPixel;
		secondPixel = newSecondPixel;
	}

	// Iterate through the destination image, pixel by pixel.
	unsigned char *data = destination.data;

	// Keep track of the row and fractional row we're on.
	int row = 0;
	int rowFraction = 0;

	for(int y = 0; y < destination.height; ++y)
	{
		// For each entry in the slopeDataArray, draw a smooth gradient slopeData->columns
		// pixels long, starting with value slopeData->firstValue and incrementing by slopeData->slopeH
		slopeData = slopeDataArray.data;
		long value = slopeData->firstValue;

		while(true)
		{
			for(int x = 0; x < slopeData->columns; ++x)
			{
				*data = (unsigned char)(value >> 16);
				++data;

				value += slopeData->slopeH;
			}

			++slopeData;
			if(slopeData == slopeDataEnd) break;

			value = slopeData->firstValue;
		}

		rowFraction += sourceHeight;
		if(rowFraction >= destination.height)
		{
			rowFraction -= destination.height;
			++row;

			firstData = source.data + row * source.width;
			secondData = firstData + source.width;

			firstPixel = (*firstData << 16);
			secondPixel = (*secondData << 16);

			for(slopeData = slopeDataArray.data; slopeData != slopeDataEnd; ++slopeData)
			{
				++firstData;
				++secondData;
				
				long newFirstPixel = (*firstData << 16);
				long newSecondPixel = (*secondData << 16);

				slopeData->slopeH = slopeData->nextSlopeH;
				slopeData->nextSlopeH = (sourceWidth * (newSecondPixel - secondPixel)) / destination.width;
				slopeData->deltaSlopeH = (sourceHeight * (slopeData->nextSlopeH - slopeData->slopeH)) / destination.height;
				slopeData->slopeV = (sourceHeight * (secondPixel - firstPixel)) / destination.height;
				slopeData->firstValue = firstPixel + (rowFraction * slopeData->slopeV) / destination.height + (slopeData->slopeH * slopeData->fraction) / destination.width;

				firstPixel = newFirstPixel;
				secondPixel = newSecondPixel;
			}
		}
		else
		{
			for(slopeData = slopeDataArray.data; slopeData != slopeDataEnd; ++slopeData)
			{
				slopeData->slopeH += slopeData->deltaSlopeH;
				slopeData->firstValue += slopeData->slopeV;
			}
		}
	}
}
/*
void Interpolator::operator()(const GreyImage &source, GreyImage &destination)
{
	int sourceWidth = source.width - 1;
	int sourceHeight = source.height - 1;

	slopeDataArray.Size(sourceWidth, 1);

	// Calculate the first set of horizontal and vertical slopes.
	unsigned char *firstData = source.data;
	unsigned char *secondData = firstData + source.width;

	long firstPixel = (*firstData << 16);
	long secondPixel = (*secondData << 16);

	SlopeData *slopeData = slopeDataArray.data;

	for(int i = 0; i < sourceWidth; ++i)
	{
		++firstData;
		++secondData;
		
		long newFirstPixel = (*firstData << 16);
		long newSecondPixel = (*secondData << 16);

		slopeData->slopeH = (sourceWidth * (newFirstPixel - firstPixel)) / destination.width;
		slopeData->nextSlopeH = (sourceWidth * (newSecondPixel - secondPixel)) / destination.width;
		slopeData->deltaSlopeH = (sourceHeight * (slopeData->nextSlopeH - slopeData->slopeH)) / destination.height;
		slopeData->slopeV = (sourceHeight * (secondPixel - firstPixel)) / destination.height;
		slopeData->firstValue = firstPixel;

		firstPixel = newFirstPixel;
		secondPixel = newSecondPixel;

		++slopeData;
	}

	unsigned char *data = destination.data;
	int row = 0;
	int rowFraction = 0;
	for(int y = 0; y < destination.height; ++y)
	{
		slopeData = slopeDataArray.data;
		long value = slopeData->firstValue;

		int fraction = 0;
		for(int x = 0; x < destination.width; ++x)
		{
			*data = (unsigned char)(value >> 16);
			++data;

			fraction += sourceWidth;
			if(fraction >= destination.width)
			{
				fraction -= destination.width;
				++slopeData;

				value = slopeData->firstValue + (slopeData->slopeH * fraction) / destination.width;
			}
			else
			{
				value += slopeData->slopeH;
			}
		}

		rowFraction += sourceHeight;
		if(rowFraction >= destination.height)
		{
			rowFraction -= destination.height;
			++row;

			unsigned char *firstData = source.data + row * source.width;
			unsigned char *secondData = firstData + source.width;

			long firstPixel = (*firstData << 16);
			long secondPixel = (*secondData << 16);

			SlopeData *slopeData = slopeDataArray.data;

			for(int i = 0; i < sourceWidth; ++i)
			{
				++firstData;
				++secondData;
				
				long newFirstPixel = (*firstData << 16);
				long newSecondPixel = (*secondData << 16);

				slopeData->slopeH = slopeData->nextSlopeH;
				slopeData->nextSlopeH = (sourceWidth * (newSecondPixel - secondPixel)) / destination.width;
				slopeData->deltaSlopeH = (sourceHeight * (slopeData->nextSlopeH - slopeData->slopeH)) / destination.height;
				slopeData->slopeV = (sourceHeight * (secondPixel - firstPixel)) / destination.height;
				slopeData->firstValue = firstPixel + (rowFraction * slopeData->slopeV) / destination.height;

				firstPixel = newFirstPixel;
				secondPixel = newSecondPixel;

				++slopeData;
			}
		}
		else
		{
			slopeData = slopeDataArray.data;
			for(int i = 0; i < sourceWidth; ++i)
			{
				slopeData->slopeH += slopeData->deltaSlopeH;
				slopeData->firstValue += slopeData->slopeV;

				++slopeData;
			}
		}
	}
}

*/

/*
void Interpolator::operator()(const GreyImage &source, GreyImage &destination)
{
	int sourceWidth = source.width - 1;
	int sourceHeight = source.height - 1;

	slopeH.Size(sourceWidth, 1);
	nextSlopeH.Size(sourceWidth, 1);
	deltaSlopeH.Size(sourceWidth, 1);
	slopeV.Size(sourceWidth, 1);
	firstValue.Size(sourceWidth, 1);

	// Calculate the first set of horizontal and vertical slopes.
	unsigned char *firstRow = source.data;
	unsigned char *secondRow = firstRow + source.width;
	for(int i = 0; i < sourceWidth; ++i)
	{
		int riseH = firstRow[1] - firstRow[0];
		int nextRiseH = secondRow[1] - secondRow[0];
		int riseV = secondRow[0] - firstRow[0];

		slopeH.data[i] = ((sourceWidth * riseH) << 16) / destination.width;
		nextSlopeH.data[i] = ((sourceWidth * nextRiseH) << 16) / destination.width;
		deltaSlopeH.data[i] = (sourceHeight * (nextSlopeH.data[i] - slopeH.data[i])) / destination.height;
		slopeV.data[i] = ((sourceHeight * riseV) << 16) / destination.height;
		firstValue.data[i] = firstRow[0] << 16;

		++firstRow;
		++secondRow;
	}

	unsigned char *data = destination.data;
	int row = 0;
	int rowFraction = 0;
	for(int y = 0; y < destination.height; ++y)
	{
		long value = firstValue.data[0];

		int index = 0;
		int fraction = 0;
		for(int x = 0; x < destination.width; ++x)
		{
			*data = (unsigned char)(value >> 16);
			++data;

			fraction += sourceWidth;
			if(fraction >= destination.width)
			{
				fraction -= destination.width;
				++index;

				value = firstValue.data[index] + (slopeH.data[index] * fraction) / destination.width;
			}
			else
			{
				value += slopeH.data[index];
			}
		}

		rowFraction += sourceHeight;
		if(rowFraction >= destination.height)
		{
			rowFraction -= destination.height;
			++row;

			unsigned char *firstRow = source.data + row * source.width;
			unsigned char *secondRow = firstRow + source.width;
			for(int i = 0; i < sourceWidth; ++i)
			{
				int nextRiseH = secondRow[1] - secondRow[0];
				int riseV = secondRow[0] - firstRow[0];

				slopeH.data[i] = nextSlopeH.data[i];
				nextSlopeH.data[i] = ((sourceWidth * nextRiseH) << 16) / destination.width;
				deltaSlopeH.data[i] = (sourceHeight * (nextSlopeH.data[i] - slopeH.data[i])) / destination.height;
				slopeV.data[i] = ((sourceHeight * riseV) << 16) / destination.height;
				firstValue.data[i] = (firstRow[0] << 16) + (rowFraction * slopeV.data[i]) / destination.height;

				++firstRow;
				++secondRow;
			}
		}
		else
		{
			for(int i = 0; i < sourceWidth; ++i)
			{
				slopeH.data[i] += deltaSlopeH.data[i];
				firstValue.data[i] += slopeV.data[i];
			}
		}
	}
}
*/
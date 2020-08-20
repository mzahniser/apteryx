/* Image.cpp
Michael Zahniser, 6/20/05

Function definitions for the Image class.
*/

#ifndef __MZ_IMAGE_CPP__
#define __MZ_IMAGE_CPP__



#include "Image.h"

#include <cstring>

using std::memset;
using std::memcpy;



// Default constructor:
template <class Type>
Image<Type>::Image()
	: width(0), height(0), data(NULL)
{
}



// Constructor:
template <class Type>
Image<Type>::Image(unsigned short newWidth, unsigned short newHeight)
	: width(newWidth), height(newHeight)
{
	if(width * height)
		data = new Type[width * height];
	else
		data = NULL;
}



// Copy constructor:
template <class Type>
Image<Type>::Image(const Image<Type> &image)
	: width(image.width), height(image.height)
{
	if(width * height)
	{
		data = new Type[width * height];
	
		memcpy(data, image.data, sizeof(Type) * Size());
	}
	else
		data = NULL;
}



// Destructor:
template <class Type>
Image<Type>::~Image()
{
	delete [] data;
}



// Size:
// Get the size of the image:
template <class Type>
unsigned long Image<Type>::Size() const
{
	return width * height;
}



// Size:
// Set the size of the image:
template <class Type>
void Image<Type>::Size(unsigned short w, unsigned short h)
{
	if(w * h != Size())
	{
		delete [] data;
		if(w * h)
			data = new Type[w * h];
		else
			data = NULL;
	}

	width = w;
	height = h;
}



// Size:
// Set the size of the image:
template <class Type>
void Image<Type>::Size(const Image<Type> &image)
{
	if(image.Size() != Size())
	{
		delete [] data;
		if(image.Size())
			data = new Type[image.Size()];
		else
			data = NULL;
	}

	width = image.width;
	height = image.height;
}



// operator=:
// Assignment
template <class Type>
Image<Type> &Image<Type>::operator=(const Image<Type> &image)
{
	Size(image);

	memcpy(data, image.data, Size());

	return *this;
}




// SwapData:
// Swap this image's data with another's.
template <class Type>
void Image<Type>::SwapData(Image<Type> &image)
{
	Type *temp = data;

	data = image.data;

	image.data = temp;

	unsigned short dim = image.width;
	image.width = width;
	width = dim;

	dim = image.height;
	image.height = height;
	height = dim;
}



// StealData:
// "Steal" another image's data
template <class Type>
void Image<Type>::StealData(Image<Type> &image)
{
	delete [] data;

	data = image.data;
	width = image.width;
	height = image.height;

	image.data = NULL;
	image.width = 0;
	image.height = 0;
}


// Erase:
template <class Type>
void Image<Type>::Erase()
{
	memset(data, 0, sizeof(Type) * Size());
}



// Erase:
template <class Type>
void Image<Type>::Erase(unsigned char value)
{
	memset(data, value, sizeof(Type) * Size());
}


#endif
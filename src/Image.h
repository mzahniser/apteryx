/* Image.h
Michael Zahniser, 6/20/05

Templated Image class
*/

#ifndef __MZ_IMAGE_H__
#define __MZ_IMAGE_H__



template <class Type>
class Image {
public:
	// Default Constructor
	Image();

	// Constructor:
	Image(unsigned short, unsigned short);

	// Copy Constructor:
	Image(const Image<Type> &);
	
	// Destructor:
	~Image();



	// Get the size of the image:
	unsigned long Size() const;

	// Set the size of the image:
	void Size(unsigned short, unsigned short);

	// Set the size of the image:
	void Size(const Image<Type> &);

	// Assignment
	virtual Image<Type> &operator=(const Image<Type> &);


	// Iterators for processing individual rows or entire image.
	const unsigned char *Begin() const { return data; }
	unsigned char *Begin() { return data; }
	const unsigned char *End() const { return data + width * height; }
	unsigned char *End() { return data + width * height; }
	const unsigned char *Begin(int row) const { return data + row * width; }
	unsigned char *Begin(int row) { return data + row * width; }
	const unsigned char *End(int row) const { return data + (row + 1) * width; }
	unsigned char *End(int row) { return data + (row + 1) * width; }

	// Access a given pixel value.
	unsigned char operator()(int x, int y) const { return data[x + y * Pitch()]; }
	unsigned char &operator()(int x, int y) { return data[x + y * Pitch()]; }

	// Image dimensions.
	int Width() const { return width; }
	int Columns() const { return width; }
	int Height() const { return height; }
	int Rows() const { return height; }
	int Pitch() const { return width; }


	// Swap this image's data with another's
	void SwapData(Image<Type> &);

	// "Steal" another image's data
	void StealData(Image<Type> &);



	// Erase
	void Erase();
	void Erase(unsigned char);



	// Image data:
	unsigned short width;
	unsigned short height;
	Type *data;
};

// Necessary for a templated class:
#include "Image.cpp"

typedef Image<unsigned char> GreyImage;

#endif
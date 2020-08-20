/* Map.h
Michael Zahniser, 07/08/07

Header file for the Map class, which stores the map of a single level.

Map provides functions for serializing the map data in human-readable form.
*/

#ifndef __MZ_MAP_H__
#define __MZ_MAP_H__


#include "DataFile.h"

#include <vector>
#include <ostream>

using std::vector;
using std::ostream;



class Map {
public:
	Map();
	Map(DataFile &in);

	// Read an entire level from the given input stream.
	void Read(DataFile &in);
	friend ostream &operator<<(ostream &out, const Map &map);
	
	// Get the level dimensions.
	int Width() const;
	int Height() const;

	// Get the index of the tile at the given (x, y) coordinates.
	unsigned char operator()(int x, int y) const;
	unsigned char &operator()(int x, int y);
	
	operator bool() const { return !!*this; }
	bool operator!() const { return (width * height == 0); }
	
	
private:
	// Get the index in the vector of the given (x, y) point.
	// If the point is out of bounds, throw an error.
	int MapIndex(int x, int y) const;
	// Read a single row from the map. Return the row width.
	int ReadRow(DataFile &in);
	
	
private:
	vector<unsigned char> mapData;
	
	int width;
	int height;
};

#endif
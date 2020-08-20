/* Map.cpp
Michael Zahniser, 07/08/07

Function definitions for the Map class, which stores the map of a single level.
*/

#include "Map.h"

#include <stdexcept>

using std::runtime_error;



Map::Map()
	: width(0), height(0)
{
}



Map::Map(DataFile &in)
{
	Read(in);
}



// Read an entire level from the given input stream.
void Map::Read(DataFile &in)
{
	mapData.clear();
	
	// A map always begins with a new line and ends with a blank line.
	while(in.get() != '\n') {}
	
	height = 0;
	width = ReadRow(in);
	
	// The map may begin on this row, or the next one.
	if(!width)
		return;
	
	while(true)
	{
		++height;
		int rowWidth = ReadRow(in);
		
		if(!rowWidth)
			return;
		if(rowWidth != width)
			throw runtime_error("Badly formed map data block.");
	}
}



ostream &operator<<(ostream &out, const Map &map)
{
	out<<"\tmap";
	
	// Write each line of the map.
	vector<unsigned char>::const_iterator it = map.mapData.begin();
	
	for(int y = 0; y < map.height; ++y)
	{
		out<<"\n\t\t";
		for(int x = 0; x < map.width; ++x)
		{
			out.put('A' + *it / 18);
			out.put('a' + *it % 18);
			++it;
		}
	}
	out<<"\n\n";
	
	return out;
}



// Get the level dimensions.
int Map::Width() const
{
	return width;
}



int Map::Height() const
{
	return height;
}



// Get the index of the tile at the given (x, y) coordinates.
unsigned char Map::operator()(int x, int y) const
{
	if((x < 0) | (y < 0) | (x >= width) | (y >= height))
		throw runtime_error("Map index out of bounds.");
		
	return mapData[MapIndex(x, y)];
}



unsigned char &Map::operator()(int x, int y)
{
	if((x < 0) | (y < 0) | (x >= width) | (y >= height))
		throw runtime_error("Map index out of bounds.");
		
	return mapData[MapIndex(x, y)];
}



// Get the index in the vector of the given (x, y) point.
// If the point is out of bounds, throw an error.
int Map::MapIndex(int x, int y) const
{
	if((x < 0) | (y < 0) | (x >= width) | (y >= height))
		throw runtime_error("Map index out of bounds.");
	
	return x + y * width;	
}



// Read a single row from the map. Return the row width.
int Map::ReadRow(DataFile &in)
{
	// Read leading whitespace. If the line ends, this is a zero-length row
	// (which probably means we're at the end of the map).
	while(in && in.peek() <= ' ')
		if(in.get() == '\n')
			return 0;
	
	// Bail out if we've reached the end of the file.
	if(!in)
		return 0;
	
	// Read one row of map data.
	// TODO: throw an error if data is malformed (unrecognized characters).
	int rowWidth = 0;
	for( ; in && in.peek() > ' '; ++rowWidth)
	{
		unsigned char value = static_cast<unsigned char>(in.get() - 'A') * 18;
		value += static_cast<unsigned char>(in.get() - 'a');
		mapData.push_back(value);		
	}
	
	// Read until the end of the line.
	while(in && in.get() != '\n') {}

	return rowWidth;
}

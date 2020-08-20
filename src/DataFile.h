/* DataFile.h
Michael Zahniser, 10/06/07

Header file for the DataFile class, which represents a file where data is stored
as a series of strings (tags) and associated numerical values.

With a standard istream, attempting to extract a numeric value when the next character
is non-numeric results in the entire istream being invalidated. This class instead just
returns a zero in that case. This allows data files to have optional values that are
omitted when their value is zero.
*/

#ifndef __MZ_DATA_FILE_H__
#define __MZ_DATA_FILE_H__


#include <fstream>
#include <string>

using std::ifstream;
using std::string;



class DataFile : public ifstream {
public:
	DataFile() {}
	DataFile(const string &path) : ifstream(path.c_str()) {}
	
	int ReadInt();
	float ReadFloat();
	bool HasNumber();
	
	
private:
	void SkipWhiteSpace();
	
	bool NextIsNumeric();
	int NextDigit();
};

#endif
/* DataFile.cpp
Michael Zahniser, 10/06/07

Function definitions for the DataFile class.
*/

#include "DataFile.h"



int DataFile::ReadInt()
{
	SkipWhiteSpace();
	
	if(!*this)
		return 0;
	
	int value = 0;
	
	// Check whether the number starts with a negative sign.
	bool negative = false;
	if(peek() == '-')
	{
		negative = true;
		get();
	}
	
	while(NextIsNumeric())
	{
		value *= 10;
		value += NextDigit();
	}
	
	return value * (!negative - negative);
}



float DataFile::ReadFloat()
{
	SkipWhiteSpace();
	
	if(!*this)
		return 0.0f;
	
	int value = 0;
	int divisor = 1;
	int decimalMultiplier = 1;
	
	// Check whether the number starts with a negative sign.
	bool negative = false;
	if(peek() == '-')
	{
		negative = true;
		get();
	}
	
	while(true)
	{
		if(peek() == '.')
		{
			decimalMultiplier = 10;
			get();
		}
		
		if(!NextIsNumeric())
			break;
		
		value *= 10;
		value += NextDigit();
		
		divisor *= decimalMultiplier;
	}
	
	return static_cast<float>(value * (!negative - negative)) / static_cast<float>(divisor);
}



bool DataFile::HasNumber()
{
	SkipWhiteSpace();

	return ((peek() >= '0') & (peek() <= '9')) | (peek() == '-') | (peek() == '.');
}


	
void DataFile::SkipWhiteSpace()
{
	while(*this && peek() <= ' ')
		get();
}



	
bool DataFile::NextIsNumeric()
{
	return (peek() >= '0') & (peek() <= '9');
}



int DataFile::NextDigit()
{
	return get() - '0';
}


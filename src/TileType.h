/* TileType.h
Michael Zahniser, 07/07/07

Stores information on a given tile type.
*/

#ifndef __MZ_TILE_TYPE_H__
#define __MZ_TILE_TYPE_H__


#include "DataFile.h"

#include <ostream>

using std::ostream;



class TileType {
public:
	TileType();
	TileType(DataFile &in);

	void Read(DataFile &in);
	friend ostream &operator<<(ostream &out, const TileType &tileType);
	
	// Get the identifier index of this tile type.
	int Index() const { return static_cast<int>(index); }
	
	int SpriteIndex(int animationStep) const;
	
	bool IsBlocked() const;
	bool IsOpaque() const;
	unsigned char MaterialIndex() const;
	
	bool IsTunnelUp() const;
	bool IsTunnelDown() const;
	
	bool IsSwitch() const;
	unsigned char FieldTag() const;
	unsigned char Transition() const;
	
	
private:
	unsigned char index;
	unsigned char spriteIndex;
	unsigned char frames;
	unsigned char material;
	char tunnel;
	unsigned char switchTag;
	unsigned char fieldTag;
	unsigned char transition;
};

#endif
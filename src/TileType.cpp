/* TileType.cpp
Michael Zahniser, 07/07/07

Stores information on a given tile type.
*/

#include "TileType.h"

#include <string>

using std::string;



TileType::TileType()
	: spriteIndex(255), frames(1), material(0),
	tunnel(0), switchTag(0), fieldTag(0), transition(255)
{
}



TileType::TileType(DataFile &in)
	: spriteIndex(255), frames(1), material(0),
	tunnel(0), switchTag(0), fieldTag(0), transition(255)
{
	Read(in);
}



void TileType::Read(DataFile &in)
{
	index = in.ReadInt();
	
	string tag;
	in>>tag;
	
	while(tag != "end")
	{
		if(tag == "sprite")
			spriteIndex = in.ReadInt();
		else if(tag == "frames")
			frames = in.ReadInt();
		else if(tag == "material")
			material = in.ReadInt();
		else if(tag == "tunnel")
			tunnel = in.ReadInt();
		else if(tag == "switch")
			switchTag = in.ReadInt();
		else if(tag == "field")
			fieldTag = in.ReadInt();
		else if(tag == "transition")
			transition = in.ReadInt();
	
		in>>tag;
	}
	
	// If a switch is triggered, all switches should change in addition to all
	// the fields. So for switches, copy the switch tag into the field tag.
	if(switchTag)
		fieldTag = switchTag;	
}



ostream &operator<<(ostream &out, const TileType &tileType)
{
	// If this is just a placeholder, don't output it.
	if(tileType.index == 255)
		return out;
	
	out<<"tile "<<tileType.Index();
	if(tileType.spriteIndex != 255)
		out<<"\n\tsprite "<<tileType.spriteIndex;
	if(tileType.frames != 1)
		out<<"\n\tframes "<<static_cast<int>(tileType.frames);
	if(tileType.material)
		out<<"\n\tmaterial "<<static_cast<int>(tileType.material);
	if(tileType.tunnel)
		out<<"\n\ttunnel "<<static_cast<int>(tileType.tunnel);
	if(tileType.switchTag)
		out<<"\n\tswitch "<<static_cast<int>(tileType.switchTag);
	if(tileType.fieldTag && !tileType.switchTag)
		out<<"\n\tfield "<<static_cast<int>(tileType.fieldTag);
	if(tileType.transition != 255)
		out<<"\n\ttransition "<<static_cast<int>(tileType.transition);
	out<<"\nend\n\n";
	
	return out;
}



int TileType::SpriteIndex(int animationStep) const
{
	return spriteIndex + (animationStep % frames);
}



bool TileType::IsBlocked() const
{
	return (material != 0);
}



bool TileType::IsOpaque() const
{
	return IsBlocked() & (!fieldTag | (switchTag != 0));
}



unsigned char TileType::MaterialIndex() const
{
	return material;
}



bool TileType::IsTunnelUp() const
{
	return (tunnel < 0);
}



bool TileType::IsTunnelDown() const
{
	return (tunnel > 0);
}



bool TileType::IsSwitch() const
{
	return (switchTag != 0);
}



unsigned char TileType::FieldTag() const
{
	return fieldTag;
}



unsigned char TileType::Transition() const
{
	return transition;
}

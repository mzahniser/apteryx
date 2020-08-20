/* Material.h
Michael Zahniser, 07/07/07

Stores information on a type of material that map tiles may be made out of.
(For example, rock, metal, and force field.)
*/

#ifndef __MZ_MATERIAL_H__
#define __MZ_MATERIAL_H__


#include "DataFile.h"

#include <ostream>

using std::ostream;



class Material {
public:
	Material();
	Material(DataFile &in);

	void Read(DataFile &in);
	friend ostream &operator<<(ostream &out, const Material &material);
	
	// Get the identifier index of this material.
	int Index() const { return static_cast<int>(index); }
	
	// Get the amount of bounciness.
	// If the material has some degree of irregularity, this function will
	// return different values each time it is called.
	double Bounce() const;
	
	unsigned char SoundIndex() const;
	
	
private:
	unsigned char index;
	unsigned char bounce;
	unsigned char irregularity;
	unsigned char sound;
};

#endif
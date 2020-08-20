/* Material.cpp
Michael Zahniser, 07/07/07

Stores information on a type of material that map tiles may be made out of.
(For example, rock, metal, and force field.)
*/

#include "Material.h"

#include <string>
#include <cstdlib>

using std::string;
using std::rand;



Material::Material()
	: index(0), bounce(0), irregularity(0), sound(0)
{
}



Material::Material(DataFile &in)
	: bounce(50), irregularity(0), sound(0)
{
	Read(in);
}



void Material::Read(DataFile &in)
{
	index = in.ReadInt();
	
	string tag;
	in>>tag;
	
	while(tag != "end")
	{
		if(tag == "bounce")
			bounce = in.ReadInt();
		else if(tag == "irregularity")
			irregularity = in.ReadInt();
		else if(tag == "sound")
			sound = in.ReadInt();
	
		in>>tag;
	}
	
	if(irregularity > 100)
		irregularity = 100;	
}



ostream &operator<<(ostream &out, const Material &material)
{
	// If this is just a placeholder, don't output it.
	if(!material.index)
		return out;
	
	out<<"material "<<material.Index();
	if(material.bounce)
		out<<"\n\tbounce "<<static_cast<int>(material.bounce);
	if(material.irregularity)
		out<<"\n\tirregularity "<<static_cast<int>(material.irregularity);
	if(material.sound)
		out<<"\n\tsound "<<static_cast<int>(material.sound);
	out<<"\nend\n\n";
	
	return out;
}



// Get the amount of bounciness.
// If the material has some degree of irregularity, this function will
// return different values each time it is called.
double Material::Bounce() const
{
	double fraction = .01 * static_cast<double>(bounce);
	
	if(irregularity)
		fraction *= .01 * static_cast<double>(100 - (rand() % (irregularity + 1)));
	
	return fraction;
}



unsigned char Material::SoundIndex() const
{
	return sound;
}

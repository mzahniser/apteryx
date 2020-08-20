/* apteryx
Written for iDevGames competition in 2002.
Rewritten more recently.
*/

#include "GameEngine.h"



int main(int argc, char *argv[])
{
	string path;
	if(argv[1])
	{
		path = argv[1];
		if(path[path.length() - 1] != '/')
			path += '/';
	}
	else if(argv[0] == string("/usr/bin/apteryx") || argv[0] == string("apteryx"))
		path = "/usr/share/apteryx/";
	else
		path = "resources/";
	
	GameEngine engine(path);
	engine.Run();
	
	return 0;
}

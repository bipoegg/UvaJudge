#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
using namespace std;


int main()
{
	int width = 0;
	int height = 0;
	
	int x = 0;
	int y = 0;
	char direction = '';
	char command = '';
	
	string line;
	
	getline(cin, line);
	istringstream is(line);
	
	is >>　width >>　height;
	
	while(!cin.eof()) 
	{
		// get position
		getline(cin, line);
	    istringstream is(line);
		line >> x >> y >> direction;
		
		// get command
		getline(cin, line);
		is = isstringstream(line);
		while(!is.eof()) 
		{
			is >> command;
		}
	}

    return 0;
}
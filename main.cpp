/*********************************************************************************
 **********************************************************************************
 * Name: William Boyd, Dai Ho, Thao Tran
 * Date : 11/17/2017 FALL 2017
 * Course: ECE485
 * Final Project
 * File name: main.cpp
 * Description: 
 **********************************************************************************
 *********************************************************************************/

#include "cache.h"

using namespace std;

L1_cache  my_dCache(numOfSets, dWays);
L1_cache my_iCache(numOfSets, iWays);

int main(int argc, const char * argv[])
{
    int command;
	char * address = new char[SIZE];
	ifstream filein;

	filein.open(argv[1]);

	if (!filein.is_open())
		return 0;

	while (read_file(filein, command, address))
	{
		cout << command << ' ' << address << endl;
        cout <<  hexToInt(address) << endl;
        cout << getTag(hexToInt(address)) << endl;
        cout << getIndex(hexToInt(address)) << endl;
        
		if (command == 0)
			my_dCache.readRequest(address);
		/*
		else if (command == 1)
			call write function
		else if (command == 2)
			call fectch instruction
		else if (command  == 3)
			call invalidate function
		else if (command == 4)
			call dataRequestFromL2
		else if (command == 8)
			call clear function
		else if (command == 9)
			call print function
		else 
			cout << "INVALID COMMAND!!!\n";
		*/
	}

    return 0;
}

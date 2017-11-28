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
	char * address =  new char[SIZE];;
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
			my_dCache.writeRequest(address);*/
		else if (command == 2)
			my_iCache.instructionFetch(address);
		else if (command  == 3)
			my_dCache.invalidateCommand(address);
		/*
		else if (command == 4)
			my_dCache.dataRequestFromL2(address);
		*/
		else if (command == 8)
		{
			my_dCache.clear();
			my_iCache.clear();
		}
		else if (command == 9)
		{
            cout << "-------------- Data Cache Content --------------" << endl;
            my_dCache.print();
            cout << "----------- Instruction Cache Content ----------" << endl;
            my_iCache.print();
		}
		else 
			cout << "INVALID COMMAND!!!\n";
	}
		
	//my_dCache._print(address); //For debugging purpose
	
	cout << "-------------- Data Cache Statistic ------------" << endl;
	my_dCache.summary();
	cout << "----------- Instruction Cache Statistic --------" << endl;
	my_iCache.summary();

    return 0;
}

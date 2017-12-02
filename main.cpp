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

	//give usage message
	if (argc != 3)
	{
		cout << "Usage: ./cache <-v> <filename>\n\n";	
		return 0;	
	}	
	else if (atoi(argv[1]) != 0)
	{
		my_dCache.set_verbose(true);
		my_iCache.set_verbose(true);
		
	}

	filein.open(argv[2]);

	if (!filein.is_open())
	{
		cout << "File does not exist!!!\n";
		return 0;
	}

	while (read_file(filein, command, address))
	{
		//cout << command << ' ' << address << endl;
        //cout <<  hexToInt(address) << endl;
        //cout << getTag(hexToInt(address)) << endl;
        //cout << getIndex(hexToInt(address)) << endl;
        
	
		switch(command)
        {
            case 0:
			{
                my_dCache.readRequest(address);
                break;
            }
            case 1: 
			{
            	my_dCache.writeRequest(address);
                break;
            }
            case 2:
			{
            	my_iCache.instructionFetch(address);
                break;
            }
            case 3:
            {
            	my_dCache.invalidateCommand(address);
            	break;
            }        
            case 4: 
			{
                my_dCache.dataRequestFromL2(address);
                break;
            }
            case 8: 
			{
               	my_dCache.clear();
                my_iCache.clear();
                break;
            }
            case 9:
			{
                cout << "-------------- Data Cache Content --------------" << endl;
                my_dCache.print();
                cout << "----------- Instruction Cache Content ----------" << endl;
                my_iCache.print();
                break;
            }
            default: 
			{
			    cout << "INVALID COMMAND!!!\n";
                break;
            }
        }
	}
		
		cout << "-------------- Data Cache Statistic ------------" << endl;
		my_dCache.summary();
		cout << "----------- Instruction Cache Statistic --------" << endl;
		my_iCache.summary();

   		return 0;
}

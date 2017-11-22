/*********************************************************************************
 *********************************************************************************
 * Name: William Boyd, Dai Ho, Thao Tran
 * Date : 11/17/2017 FALL 2017
 * Course: ECE485
 * Final Project
 * File name: cache.cpp
 * Description: This file contains the definitions of the cache's member functions
 along with its constructor
 *********************************************************************************
 *********************************************************************************/

#include "cache.h"

/***************************** CONSTRUCTOR & DESTRUCTOR *************************/
cacheLine::cacheLine(void)
{
	address = NULL;
	tag = 0;
	mesiBit = 'I';	
	LRU = 0;
}

//destructor
cacheLine::~cacheLine(void)
{
	delete address;
	address = NULL;
}

L1_cache::L1_cache(void)
{
	sets = NULL;
	hit = 0;
	miss = 0;
	read = 0;
	write = 0;
	hitRate = 0;
};


L1_cache::L1_cache(int set, int way)
{
	sets = new cacheLine *[set];

	for (int i = 0; i < set; i++)
	{
		sets[i] = new cacheLine[way];
	}
}

L1_cache::~L1_cache(void)
{
	delete [] sets;
}

/******************************* Read File *************************************/
int read_file(ifstream & filein, int & command, char * address)
{
	int end = 0;
	char to_get_address[SIZE];

	filein >> command;
	filein.ignore(100, ' ');

	if (!filein.eof())
	{
		filein.get(to_get_address, 100, '\n');
		filein.ignore(100, '\n');
		strcpy(address, to_get_address);
		end = 1;
	}
	
	return end;
}

/******************************* HexToInt **************************************/
long int hexToInt(char * address)
{
    return strtol(address, NULL, 16);
}

/******************************* Get Index *************************************/
long int getIndex(long int intAddress)
{
    return (intAddress >> (int)log2(sizeLine)) & (long int)(pow(2, log2(numOfSets)) - 1);
}

/******************************* Get Tag ***************************************/
long int getTag(long int intAddress)
{
    int shiftBit;
    shiftBit = log2(sizeLine) + log2(numOfSets);
    return intAddress >> shiftBit;
}

/******************************* Read Request **********************************/
void L1_cache::readRequest(char * to_read_address)
{
	long int index;
	
	index = getIndex(hexToInt(to_read_address));
	
	for (int i = 0; i < dWays; i++)
	{	
		if (sets[index][i].mesiBit == 'I')
		{
			sets[index][i].address = new char[strlen(to_read_address) + 1];
			strcpy(sets[index][i].address, to_read_address);
			sets[index][i].tag = getTag(hexToInt(to_read_address));
			sets[index][i].mesiBit = 'S';
		}
		cout << sets[index][i].address << endl; 
		cout << sets[index][i].tag << endl;
		cout << sets[index][i].mesiBit << endl;
	}
}

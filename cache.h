/*********************************************************************************
 **********************************************************************************
 * Name: William Boyd, Dai Ho, Thao Tran
 * Date : 11/17/2017 FALL 2017
 * Course: ECE485
 * Final Project
 * File name: cache.h
 * Description: This file contains the decription of cache class
 **********************************************************************************
 *********************************************************************************/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iomanip>

using namespace std;

const int iWays         = 2;            //I cache 2 ways associative
const int dWays         = 4;            //D cache 4 ways associative
const int numOfSets     = 16 * 1024;    //number of sets
const int sizeAddress   = 32;           //size of address
const int sizeLine      = 64;           //size of a cache line data
const int SIZE 			= 100;

//This class  contains content of a cache line
class cacheLine
{
public:
	cacheLine(void);
	~cacheLine(void);

    char * address;
    long int tag;
    char mesiBit;
	int LRU;
	bool isFirstWrite;
};

//L1 cache
class L1_cache
{
public:
    L1_cache(void);  //constructor
	L1_cache(int set, int way);
	~L1_cache(void);

    void readRequest(char * to_read_address);
	void writeRequest(char * to_write_address);
	bool invalidateCommand(char * to_validate_address);
    void instructionFetch(char * to_fetch_address);
	bool dataRequestFromL2(char * to_send_address);
	void clear(void);
	void print(void);
	void summary(void);
	bool set_verbose(bool to_set);

private:	
	void updateLRU(long int index, int way);
	void LRUForInval(long int index, int way);
    cacheLine ** sets;
	long int totalSets;
	int numOfWays;
    long int hit;
    long int miss;
    long int read;
    long int write;
    float hitRate;
	bool verbose;
};

/*********************************** Other Functions **********************************/
int read_file(ifstream & filein, int & command, char * address);
long int hexToInt(char * address);
long int getIndex(long int intAddress);
long int getTag(long int intAddress);

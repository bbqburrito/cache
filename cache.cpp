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
	isFirstWrite = true;
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
	totalSets = 0;
	numOfWays = 0;
	hit = 0;
	miss = 0;
	read = 0;
	write = 0;
	hitRate = 0;
	verbose = false;
};


L1_cache::L1_cache(int set, int way)
{
	totalSets = set;
	numOfWays = way;
	hit = 0;
	miss = 0;
	read = 0;
	write = 0;
	hitRate = 0;
	verbose = false;

	sets = new cacheLine *[set];

	for (int i = 0; i < set; i++)
	{
		sets[i] = new cacheLine[way];
	}
}

L1_cache::~L1_cache(void)
{
	delete [] sets;
	sets = NULL;
}

/******************************* Read File *************************************/
int read_file(ifstream & filein, int & command, char * address)
{
	int end = 0;
	char to_get_address[SIZE];

	filein >> command;

	if (!filein.eof() && (command == 9 || command == 8))
	{
		filein.ignore(100, '\n');
		return 1;
	}

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

/************************** HexToInt & IntToHEx ********************************/
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
	long int index, tag;
	
	index = getIndex(hexToInt(to_read_address));
	tag = getTag(hexToInt(to_read_address));

	//Check if there is a hit
	for (int i = 0; i < numOfWays; i++)
	{
		if (sets[index][i].mesiBit != 'I' && sets[index][i].tag == tag)
		{
			//No need to update MESI
			updateLRU(index, i);
			read++;
			hit++;
			return;
		}
	}

	//No hit, find an available cache line to read data into or evict LRU line
	//Verbose message
	if (verbose)
		cout << "<---> Read from L2 " << to_read_address << endl;

	for (int j  = 0; j < numOfWays; j++)
	{	
		if (sets[index][j].mesiBit == 'I') //There is an available line
		{
			sets[index][j].address = new char[strlen(to_read_address) + 1];
			strcpy(sets[index][j].address, to_read_address);
			sets[index][j].tag = tag;
			sets[index][j].mesiBit = 'S';
			
			updateLRU(index, j);
			read++;
			miss++;
			return;
		}
		else if (sets[index][j].LRU == 0) //Line is LRU line, evict this line
		{
			if (sets[index][j].mesiBit == 'M' && verbose)
				cout << "<---> Write data to L2 " << sets[index][j].address << endl;

			delete sets[index][j].address;
			sets[index][j].address = new char[strlen(to_read_address) + 1];
			strcpy(sets[index][j].address, to_read_address);
			sets[index][j].tag = tag;
			sets[index][j].mesiBit = 'S';
	
			updateLRU(index, j);
			read++;
			miss++;
			return;
		}
	}
}		


/******************************* Write Request **********************************/
void L1_cache::writeRequest(char * to_write_address)
{
    long int index, tag;
    
    index = getIndex(hexToInt(to_write_address));
    tag = getTag(hexToInt(to_write_address));
    
    //Check if there is a hit
    for (int i = 0; i < numOfWays; i++)
    {
        if (sets[index][i].mesiBit != 'I' && sets[index][i].tag == tag)
        {	
			if (sets[index][i].isFirstWrite)
			{
				if (verbose)
					cout << "<---> Write to L2 " << to_write_address << endl;

				sets[index][i].mesiBit = 'E';
				sets[index][i].isFirstWrite = false;
			}
			else 
				sets[index][i].mesiBit = 'M';
            
			updateLRU(index, i);
            write++;
            hit++;
            return;
        }
    }
    
    //No hit, find an available cache line to write data into or evict LRU line
	
	//verbose message to indicate a read from L2 due to a write miss
	//read++; //read for ownership

	if (verbose)
	{
		cout << "<---> Read for Ownership from L2 " << to_write_address << endl;
		cout << "<---> Write data to L2 " << to_write_address << endl;
	}

    for (int j  = 0; j < numOfWays; j++)
    {
        if (sets[index][j].mesiBit == 'I') //There is an available line
        {
            sets[index][j].address = new char[strlen(to_write_address) + 1];
            strcpy(sets[index][j].address, to_write_address);
            sets[index][j].tag = tag;
			sets[index][j].mesiBit = 'E';
			sets[index][j].isFirstWrite = false;

            updateLRU(index, j);
            write++;
            miss++;
            return;
        }
        else if (sets[index][j].LRU == 0) //Line is LRU line, evict this line
        {
			if (sets[index][j].mesiBit == 'M' && verbose)
				cout << "<---> Write data to L2 " << sets[index][j].address << endl;

            delete sets[index][j].address;
            sets[index][j].address = new char[strlen(to_write_address) + 1];
            strcpy(sets[index][j].address, to_write_address);
            sets[index][j].tag = tag;
            sets[index][j].mesiBit = 'E';
            sets[index][j].isFirstWrite = false;

            updateLRU(index, j);
            write++;
            miss++;
            return;
        }
    }

	return;
}



/******************************* Invalid Command ********************************/
bool L1_cache::invalidateCommand(char * to_validate_address)
{
	bool rValue = false;

	long int index, tag;
	index = getIndex(hexToInt(to_validate_address));
	tag = getTag(hexToInt(to_validate_address));

	//Check if to_validate_address is in a cache line
	for (int i = 0; i < numOfWays; i++)
	{
		if (sets[index][i].mesiBit != 'I' && sets[index][i].tag == tag)
		{	
			//Return data to L2 if it has been modified
			if (sets[index][i].mesiBit == 'M' && verbose)
				cout << "<---> Write data to L2 " << sets[index][i].address << endl;

			LRUForInval(index, i);
			sets[index][i].mesiBit = 'I';
			sets[index][i].isFirstWrite = true;
			delete sets[index][i].address;
			
			rValue = true;
			break;
		}
	}
	
	//to_validate_address is not in a cache line then simply return
	return rValue;
}


/**************************** Instruction Fetch  ********************************/
void L1_cache::instructionFetch(char * to_fetch_address)
{
	readRequest(to_fetch_address);

	return;
}


/************************** Data Request From L2  *******************************/
bool L1_cache::dataRequestFromL2(char * to_send_address)
{
	bool rValue = false;
	long int index, tag;
	index = getIndex(hexToInt(to_send_address));
	tag = getTag(hexToInt(to_send_address));
	
	//Check if to_send_address is in a cache line
	for (int i = 0; i < numOfWays; i++)
	{
		if (sets[index][i].mesiBit == 'M' && sets[index][i].tag == tag)
		{
			if (verbose)
				cout << "<---> Return data to L2 " << sets[index][i].address << endl;

			sets[index][i].mesiBit = 'S';
			rValue = true;
			break;
		}
	}
	
	//to_send_address is not in a cache line then simply return false
	return rValue;
}




/********************************** Clear ***************************************/
void L1_cache::clear(void)
{
	read = 0;
	write = 0;
	hit = 0;
	miss = 0;
	hitRate = 0;

	for (int i = 0; i < totalSets; i++)
	{
		for (int j = 0; j < numOfWays; j++)
		{
			if (sets[i][j].mesiBit != 'I')
			{
				sets[i][j].mesiBit = 'I';
				sets[i][j].LRU = 0;
				sets[i][j].tag = 0; //Not really need this
				sets[i][j].isFirstWrite = true;
				delete sets[i][j].address;	
			}
		}
	}

	return;
}

/*********************************** Print  *************************************/
void L1_cache::print(void)
{
	for (int i = 0; i < totalSets; i++)
	{
		for (int j = 0; j < numOfWays; j++)
		{
			if (sets[i][j].mesiBit != 'I')
			{
				cout << "***[SET " << i << ']' << "[WAY " << j << "]: " << endl;
				cout << "\tAddress: "  << sets[i][j].address << endl;
				cout << "\tTag: " << sets[i][j].tag << endl;
				cout << "\tLRU: " << sets[i][j].LRU << endl;
           		cout << "\tMESI: " << sets[i][j].mesiBit << endl << endl;
			}
		}
	}

	return;
}


/*********************************** Update LRU  *********************************/
void L1_cache::updateLRU(long int index, int way)
{
	for (int i = 0; i < numOfWays; i++)
	{
		if (sets[index][i].mesiBit != 'I')
		{
			if (sets[index][i].LRU > sets[index][way].LRU)
				--sets[index][i].LRU;
		}
	}	

	sets[index][way].LRU = numOfWays - 1;
	
	return;
}

void L1_cache::LRUForInval(long int index, int way)
{
	for (int i = 0; i < numOfWays; i++)
	{
		if (sets[index][i].mesiBit != 'I')
		{
			if (sets[index][i].LRU < sets[index][way].LRU)
				++sets[index][i].LRU;
		}
	}

	sets[index][way].LRU = 0;
}


/*********************************** summary ** *********************************/
void L1_cache::summary(void)
{
	if (hit != 0 || read != 0 || write != 0) //avoid 0/0
		hitRate = (float)(hit)/(float)(read+write);

	cout << "***Number of cache reads: " << read << endl;
	cout << "***Number of cache writes: " << write << endl;
	cout << "***Number of cache hits: " << hit << endl;
	cout << "***Number of cache misses: " <<  miss << endl;
	cout << "***Cache hit ratio: " << fixed << setprecision(2) << hitRate * 100 << '%' << endl << endl;

	return;
}

bool L1_cache::set_verbose(bool to_set)
{
	verbose = to_set;
	return verbose;
}


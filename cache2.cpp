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

	filein >> std::skipws >> command;
	filein.ignore(100, ' ');

	if (!filein.eof())
	{
        filein >> std::skipes;
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
	long int index, tag;
	
	index = getIndex(hexToInt(to_read_address));
	tag = getTag(hexToInt(to_read_address));

	//Check if there is a hit
	for (int i = 0; i < numOfWays; i++)
	{
		if (sets[tag][i].mesiBit != 'I' && sets[index][i].tag == tag)
		{
			//No need to update MESI
			updateLRU(index, i);
			read++;
			hit++;
			return;
		}
	}

	//No hit, find an available cache line to read data into or evict LRU line
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
    long index;
    long tag;




	return;
}



/******************************* Invalid Command ********************************/
void L1_cache::invalidateCommand(char * to_validate_address)
{
	long int index, tag;
	index = getIndex(hexToInt(to_validate_address));
	tag = getTag(hexToInt(to_validate_address));

	//Check if to_validate_address is in a cache line
	for (int i = 0; i < numOfWays; i++)
	{
		if (sets[index][i].mesiBit != 'I' && sets[index][i].tag == tag)
		{	
			LRUForInval(index, i);
			sets[index][i].mesiBit = 'I';
			delete sets[index][i].address;
			
			return;
		}
	}
	
	//to_validate_address is not in a cache line then simply return
	return;
}



/**************************** Instruction Fetch  ********************************/
void L1_cache::instructionFetch(char * to_fetch_address)
{
	readRequest(to_fetch_address);

	return;
}



/************************** Data Request From L2  *******************************/
void L1_cache::dataRequestFromL2(char * to_send_address)
{

	int index, tag;
	

	index = getIndex(hexToInt(to_read_address));
	tag = getTag(hexToInt(to_read_address));
	return;
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
            if (sets[i][j].mesiBit == 'M')
            {
                sets[i][j].writeBack();
				sets[i][j].mesiBit = 'I';
				sets[i][j].LRU = 0;
				sets[i][j].tag = 0;
				delete sets[i][j].address;	
            }
                
			if (sets[i][j].mesiBit != 'I')
			{
				sets[i][j].mesiBit = 'I';
				sets[i][j].LRU = 0;
				sets[i][j].tag = 0;
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


void L1_cache::_print(char * to_print_address) //For debugging purpose
{
	long int index;

	index = getIndex(hexToInt(to_print_address));
	cout << "------------- SET " << index << " -------------\n";

	for (int j = 0; j < numOfWays; j++)
	{	
		if (sets[index][j].mesiBit != 'I')
		{
			cout << "*Way " << j << ": " << endl;
			cout << "Address: " << sets[index][j].address << endl;
			cout << "Tag: " << sets[index][j].tag << endl;
			cout << "LRU: " << sets[index][j].LRU << endl;
			cout << "MESI: " << sets[index][j].mesiBit << endl;
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

	cout << "*Number of cache reads: " << read << endl;
	cout << "*Number of cache writes: " << write << endl;
	cout << "*Number of cache hits: " << hit << endl;
	cout << "*Number of cache misses: " <<  miss << endl;
	cout << "*Cache hit ratio: " << fixed <<setprecision(2) << hitRate * 100 << '%' << endl;

	return;
}

bool set_verbose(bool to_set)
{
    verbose = to_set;
    return verbose;
}

bool get_verbose()
{
    return verbose;
}






#include "cache.h"



int read_file(std::ifstream & filein, int & command, char * address)
{
    int end = 0;
    char * to_get_address = new char[SIZE];
    char * to_get_command = new char[SIZE];


    if(!filein.is_open())
        return end;

    filein >> command;
    filein.ignore(100, ' ');


    if(!filein.eof())
    {
        filein.get(to_get_address, 100, '\n');
        filein.ignore(100, '\n');

        //command = to_get_address[0] - 48;

        strcpy(address, to_get_address);
        end = 1;
    }

    return end;
}



int main(int argc, char* argv[])
{
    int command;
    char * address = new char[SIZE];
    std::ifstream filein;

    filein.open(argv[1]);

    if (!filein.is_open())
        return 0;

    while(read_file(filein, command, address))
    {
        std::cout << command << std::endl;
        std::cout << address << std::endl;
    }

    return 0;
}











    

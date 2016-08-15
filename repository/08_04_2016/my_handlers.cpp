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
		can_value = 5620492335629467648;
		std::cout << "RPM";
		// encodeSignal pulls the CAN signal definition from the CanSignal struct
		// and encodes the value into the right bits of a 64-bit return value.
		return can_value;
}

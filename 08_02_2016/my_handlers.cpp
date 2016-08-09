//Contains handlers for translations in commmand library.

namespace can = openxc::can;

using openxc::can::lookupSignal;


/* Round the value down to 0 if it's less than 100 before writing to CAN. */
uint64_t ourRPMWriteEncoder(CanSignal* signal, CanSignal* signals,
	int signalCount, double value, bool* send) {
		
		uint64_t can_value = 0;
		double temp = 0;

		value = value / 64; //convert rpm to decimal float
		can_value = static_cast<uint64_t>(value); //create 64-bit int value
		can_value <<= 32; //shift to CAN bytes 0 and 1
		can_value = can_value | 0x0000000028; // OR with 0x00000028

		temp = static_cast<float>(can_value);

		// encodeSignal pulls the CAN signal definition from the CanSignal struct
		// and encodes the value into the right bits of a 64-bit return value.
		return openxc::can::write::encodeNumber(signal, temp, true);
}

uint64_t ourMPHWriteEncoder(CanSignal* signal, CanSignal* signals,
	int signalCount, double value, bool* send) {
		
		uint64_t can_value = 0x0000000028; // 0mph and 0rpm
		double temp = 0;

		temp = (value * 0.6) + 40; //convert mph to decimal float
		can_value = static_cast<uint64_t>(temp); //create 64-bit int value
		can_value <<= 24; //shift to CAN byte 5

		// encodeSignal pulls the CAN signal definition from the CanSignal struct
		// and encodes the value into the right bits of a 64-bit return value.
		return can_value;
}

uint64_t ourLeftSignalWriteEncoder(CanSignal* signal, CanSignal* signals,
	int signalCount, bool value, bool* send) {

		uint64_t can_value = 0;

		if(value == true) {
			can_value = 0x4E; //turns signal ON
		}

		return can_value;
}

uint64_t ourRightSignalWriteEncoder(CanSignal* signal, CanSignal* signals,
	int signalCount, bool value, bool* send) {

		uint64_t can_value = 0;

		if(value == true) {
			can_value = 0x30C0; //turns signal ON
		}

		return can_value;
}
//Contains handlers for translations in commmand library.

namespace can = openxc::can;

using openxc::can::lookupSignal;


/* Round the value down to 0 if it's less than 100 before writing to CAN. */
uint64_t ourRPMWriteEncoder(CanSignal* signal, CanSignal* signals,
	int signalCount, double value, bool* send) {
		
		uint64_t can_value = 0;
		double temp = 0;

		// Look up the numeric signals we need to send and abort if missing
		CanSignal* rpmSignal = lookupSignal("set_rpm", signals, signalCount);
		if(rpmSignal == NULL) {
			debug("Unable to find signal, can't send rpm");
			return can_value;
		}

		temp = value;
		can_value = static_cast<uint64_t>(temp) / 64; //create 64-bit value
		can_value <<= 56; //shift to CAN bytes 0 and 1
		can_value = can_value | 655360000; // OR with 0x27100000

		// encodeSignal pulls the CAN signal definition from the CanSignal struct
		// and encodes the value into the right bits of a 64-bit return value.
		return can_value;
}

uint64_t ourBlinkerWriteEncoder(CanSignal* signal, CanSignal* signals,
	int signalCount, bool value, bool* send) {

		uint64_t can_value = 0;

		// Look up the numeric signals we need to send and abort if missing
		CanSignal* leftblinkerSignal = lookupSignal("left_blinker", signals, signalCount);
		if(rpmSignal == NULL) {
			debug("Unable to find signal, can't set blinker");
			return can_value;
		}

		if(value == true) {
			can_value = 0x4E; //turns blinker ON
		}

		return can_value;
}
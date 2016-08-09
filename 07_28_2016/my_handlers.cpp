//Contains handlers for translations in commmand library.

namespace can = openxc::can;

using openxc::can::lookupSignal;


/* Round the value down to 0 if it's less than 100 before writing to CAN. */
void ourRPMWriteEncoder(CanSignal* signal, CanSignal* signals,
	int signalCount, double value, bool* send) {
		
		// Look up the numeric signals we need to send and abort if missing
		CanSignal* rpmSignal = lookupSignal("set_rpm", signals, signalCount);
		if(rpmSignal == NULL) {
			debug("Unable to find signal, can't send rpm");
			return;
		}

		uint64_t can_value = static_cast<uint64_t>(value) / 64; //create 64-bit value
		can_value <<= 56; //shift to CAN bytes 0 and 1
		can_value = can_value | 655360000; // OR 0x27100000
		

		// encodeSignal pulls the CAN signal definition from the CanSignal struct
		// and encodes the value into the right bits of a 64-bit return value.
		openxc::can::write::sendEncodedSignal(rpmSignal, can_value, true);
		return;
	}

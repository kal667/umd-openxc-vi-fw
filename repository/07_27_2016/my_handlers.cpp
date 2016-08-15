//Contains handlers for translations in commmand library.

namespace can = openxc::can;

using openxc::can::lookupSignal;

//Change from rpm value to value that is read by CAN.
/*void handleRPM(const char* name, openxc_DynamicField* value,
	openxc_DynamicField* event, CanSignal* signals, int signalCount) {

	// Look up the numeric signals we need to send and abort if missing
	CanSignal* rpmSignal = lookupSignal("set_rpm",
		signals, signalCount);
	if(rpmSignal == NULL) {
		debug("Unable to find signal, can't send rpm");
		return;
	}

	if(value < 0){
		debug("Value is negative, can't send rpm");
		return;
	}
	
	// The user enters the desired rpm value and we 
	// convert to decimal value to be written to CAN
	
	// rpm = 64 * (decimal value)
	// decimal value = rpm / 64
	*value = *value / 64;

	// Send the rpm signal
	can::write::encodeAndSendSignal(rpmSignal, value,
		// the last parameter is true, meaning we want to force sending
		// this signal even though it's not marked writable in the
		// config
		true);

}*/

/* Round the value down to 0 if it's less than 100 before writing to CAN. */
uint64_t ourRPMWriteEncoder(CanSignal* signal, CanSignal* signals,
	int signalCount, double value, bool* send) {
		if(value < 5000) {
			value = 78;
		}
		// encodeSignal pulls the CAN signal definition from the CanSignal struct
		// and encodes the value into the right bits of a 64-bit return value.
		return openxc::can::write::encodeNumber(signal, value, send);
}

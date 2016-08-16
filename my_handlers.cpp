//Contains handlers for translations in commmand library.

namespace can = openxc::can;

using openxc::can::lookupSignal;

//Translate decimal RPM value to hex
uint64_t ourRPMWriteEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {
		
	*send = true;
	
	uint64_t can_value = value->numeric_value;

	can_value = can_value / 64;
	can_value <<= 56; //shift to CAN bytes 0 and 1
	can_value = can_value | 0x0000000028000000; // OR with 0x00000028

	return can_value;
}

//Translate decimal MPH value to hex
uint64_t ourMPHWriteEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {
		
	*send = true;
	
	uint64_t can_value = 0;
	double temp = value->numeric_value;

	temp = (temp * 0.6) + 40; //convert mph to decimal float
	can_value = static_cast<uint64_t>(temp); //create 64-bit int value
	can_value <<= 24; //shift to CAN byte 5

	return can_value;
}

//Translates percentage of heat gauge to hex
uint64_t ourEngineTempEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {
	
	*send = true;
	
	uint64_t can_value = 0;
	double temp = value->numeric_value;

	temp = (temp * 0.96) + 80; //convert mph to decimal float
	can_value = static_cast<uint64_t>(temp); //create 64-bit int value

	return can_value;
}

//Translates boolean to hex
uint64_t keepAwakeBooleanEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {

	*send = true;
	
	uint64_t can_value = 0;
	bool temp = value->boolean_value;

	if (temp == true) can_value = 0x45;

	return can_value;
}

//Translates boolean to hex
uint64_t lightsBooleanEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {

	*send = true;
	
	uint64_t can_value = 0;
	bool temp = value->boolean_value;

	if (temp == true) can_value = 0x000100000001410000;
	if (temp == false) can_value = 0x000000000001010000;

	return can_value;
}


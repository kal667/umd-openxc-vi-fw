//Contains handlers for translations in commmand library.

namespace can = openxc::can;

#include <time.h>
 
void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

using openxc::can::lookupSignal;

//Encodes decimal RPM value
uint64_t ourRPMWriteEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {
		
	*send = true;
	
	uint64_t can_value = value->numeric_value;

	can_value = can_value / 64;
	can_value <<= 56; //shift to CAN bytes 0 and 1
	can_value = can_value | 0x0000000028000000; // OR with 0x00000028

	return can_value;
}

//Encodes decimal MPH value
uint64_t ourMPHWriteEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {
		
	*send = true;
	
	uint64_t can_value = 0;
	double temp = value->numeric_value;

	temp = (temp * 0.6) + 40; //convert mph to decimal float
	can_value = static_cast<uint64_t>(temp); //create 64-bit int value
	can_value <<= 24; //shift to CAN byte 5

	return can_value;
}

//Encodes percentage of heat gauge.
uint64_t ourEngineTempEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {
	
	*send = true;
	
	uint64_t can_value = 0;
	double temp = value->numeric_value;

	temp = (temp * 0.96) + 80; //convert mph to decimal float
	can_value = static_cast<uint64_t>(temp); //create 64-bit int value

	return can_value;
}

//Encodes boolean awake status. Put this command in a while loop to run in Terminal.
uint64_t keepAwakeBooleanEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {

	*send = true;
	
	uint64_t can_value = 0;
	bool temp = value->boolean_value;

	if (temp == true) can_value = 0x45;//Keep Awake On

	return can_value;
}

//Encodes boolean light status.
uint64_t lightsBooleanEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {

	*send = true;
	
	uint64_t can_value = 0;
	bool temp = value->boolean_value;

	if (temp == true) can_value = 0xFFFF011200;//Lights ON
	if (temp == false) can_value = 0xFFFF010C40;//Lights OFF

	return can_value;
}

//Encodes string gear status.
uint64_t gearEncoder(CanSignal* signal, openxc_DynamicField* value, bool* send) {

	*send = true;
	
	uint64_t can_value = 0;
	const char* gear = value->string_value;

	if (!strcmp("R", gear)) can_value = 0xA100000000005E30;//Reverse
	if (!strcmp("N", gear)) can_value = 0xEE00000000005E50;//Neutral
	if (!strcmp("D", gear)) can_value = 0x1700000000005E70;//Drive
	if (!strcmp("M", gear)) can_value = 0x1700000000005ED2;//Manual

	return can_value;
}

//Handler to control infotainment
void infotainmentHandler(const char* name, openxc_DynamicField* value,
	openxc_DynamicField* event, CanSignal* signals, int signalCount) {
	
	CanMessage message = {0};
	CanMessage can_message = {0};
	
	uint64_t can_value = {0};
	//uint64_t reset = 0;

	const char* button = value->string_value;//this probably isn't right

	CanSignal* buttonSignal = lookupSignal("infotainment_button", signals,
		signalCount);
	//CanSignal* dummySignal = lookupSignal("set_mph", signals,
	//	signalCount);

	CanBus* bus = openxc::can::lookupBus(2, openxc::signals::getCanBuses(), openxc::signals::getCanBusCount());

	if (!strcmp("v_up", button)) {
		can_value = 11144;//0x2B88;
		can_message = {0x201, STANDARD, {0x2B, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 8};
		message = {0x2E0, STANDARD, {0x2B, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 8};
		//reset = 0x2B48;
	}

	if (!strcmp("v_down", button)) {
		can_value = 11400;//0x2C88;
		//message = {0x2E0, STANDARD, {0x2C, 0x48}, 2};
	}

	if (!strcmp("t_up", button)) {
		can_value = 10632;//0x2988;
		//message = {0x2E0, STANDARD, {0x29, 0x48}, 2};
	}

	if (!strcmp("t_down", button)) {
		can_value = 10888;//0x2A88;
		//message = {0x2E0, STANDARD, {0x2A, 0x48}, 2};
	}

	if (!strcmp("t_toggle", button)) {
		can_value = 32904;//0x8088;
		//message = {0x2E0, STANDARD, {0x80, 0x48}, 2};
	}

	can::write::sendEncodedSignal(buttonSignal, can_value, true);
	//can::write::enqueueMessage(bus, &can_message);
	//can::write::flushOutgoingCanMessageQueue(bus);
	//sleep(1000000000);
	//for (int i = 0; i < 2; i++) {
		can::write::enqueueMessage(bus, &can_message);
		can::write::flushOutgoingCanMessageQueue(bus);
		can::write::enqueueMessage(bus, &can_message);
		can::write::flushOutgoingCanMessageQueue(bus);
		can::write::enqueueMessage(bus, &can_message);
		can::write::flushOutgoingCanMessageQueue(bus);
		can::write::enqueueMessage(bus, &can_message);
		can::write::flushOutgoingCanMessageQueue(bus);
		//can::write::sendEncodedSignal(dummySignal, can_value, true);
	//}
	//can::write::sendEncodedSignal(buttonSignal, can_value, true);
	can::write::enqueueMessage(bus, &message);
	//can::write::flushOutgoingCanMessageQueue(bus);
}



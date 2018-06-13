#include "mario_js.h"
#include "native_arduino_basic.h"

void dump(const char*s) {
	Serial.print(s);  
}

void setup() {  
	Serial.begin(19200);
	delay(200);

	_debug_func = dump;
	vm_t vm;
	vm_init(&vm);
 
	reg_native_arduino_basic(&vm);

	const char* js = "Serial.print('Hello, Arduino JS!\n');";

	if(vm_load(&vm, js)) {
		Serial.print("\n****** run ***********\n");
		vm_run(&vm);
		Serial.print("\n****** ended *********\n");
	}
	vm_close(&vm);

}

void loop() {
	delay(1000);
}


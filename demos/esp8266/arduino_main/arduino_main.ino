#include <native_esp8266.h>

#include "mario_js.h"
#include "native_basic.h"
#include "native_arduino_basic.h"
#include "native_esp8266.h"
#include <FS.h>

void dump(const char*s) {
	Serial.print(s);  
}

bool load_js(vm_t* vm) {
	char* buf = NULL;

	if (!SPIFFS.begin()) {
		Serial.println("Failed to mount file system");
		return false;
	}

	File jsFile = SPIFFS.open("/main.js", "r");
	if (!jsFile) {
		Serial.println("Failed to open js file");
		return false;
	}

	size_t size = jsFile.size();
	if (size > 10240) {
		Serial.println("js file size is too large");
		return false;
	}

	// Allocate a buffer to store contents of the file.
	buf = (char*)_malloc(size+1);
	jsFile.readBytes(buf, size);
	buf[size] = 0;
	jsFile.close();

	bool ret = vm_load(vm, buf);
	_free(buf);
	return ret;
}

void setup() {  
	Serial.begin(19200);
	delay(200);

	uint32_t initHeapSize =  ESP.getFreeHeap(); 

	_debug_func = dump;
	vm_t vm;
	vm_init(&vm);

  reg_native_basic(&vm);
	reg_native_arduino_basic(&vm);
	reg_native_esp8266(&vm);

	if(load_js(&vm)) {
		Serial.print("\n****** js loaded******\n");
		Serial.printf("Heap used by loaded JS: %u bytes\n", initHeapSize - ESP.getFreeHeap());
		Serial.print("\n****** run ***********\n");

		vm_run(&vm);

		Serial.print("\n****** ended *********\n");
	}
	vm_close(&vm);

}

void loop() {
	delay(1000);
}


#include "panic.h"

void kernel_panic(char* err_msg) {
	// disable all interrupts
	disable_interrupts();

	puts(err_msg);
	puts("\nEVERYBODY PANIC\n");
	for (;;);
}

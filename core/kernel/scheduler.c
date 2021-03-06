#include "scheduler.h"

/* Private data */

static bool _thread_sleeping = false;
struct SleepingThread _sleeper;


/* Private function defs */


/* Public function impls */

void init_scheduler() {
	_thread_sleeping = false;
}

void scheduler_tick_handler() {
	IRQ_PREFACE();

	if (_thread_sleeping) {
		if (_sleeper.ticks_to_sleep > 0) {
			_sleeper.ticks_to_sleep--;
		} else {
			asm(
				"mov dword [esp + 4*10], %0\n"
				:: "r" (_sleeper.ret_addr)
			);
			_thread_sleeping = false;
		}
	}

	interruptdone(0);
	IRQ_SUFFIX();
}

void sleep(uint32_t ticks) {
	asm("cli");

	// TODO: make threadsafe
	if (_thread_sleeping) {
		panic("scheduler: tried to sleep while a thread was sleeping");
	}
	_thread_sleeping = true;

	// store return address of sleeper
	_sleeper.ticks_to_sleep = ticks;
	asm("mov %0, after\n" : "=r" (_sleeper.ret_addr));

	// halt until awoken
	asm(
		"sti\n"
		"loop: hlt\n"
		"jmp loop\n"
		"after: nop\n"
	);
	return;
}


/* Private function impls */

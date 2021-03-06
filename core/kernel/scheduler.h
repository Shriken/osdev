#pragma once

#include <hal.h>
#include <irq.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "panic.h"

void init_scheduler();
void scheduler_tick_handler();
void sleep(uint32_t ticks);

struct SleepingThread {
	void* ret_addr;
	uint32_t ticks_to_sleep;
};

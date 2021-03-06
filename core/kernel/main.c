#include "main.h"

// Size of kernel binary in 512B chunks
uint32_t kernel_size;

int kernel_main(multiboot_info* info) {
	asm("mov %0, edx\n" : "=m" (kernel_size));
	kernel_size++;

	init_rendering();
	puts("Hello, world of kernels\n\n");
	printf(
		"Our kernel is this big: %d sectors = %d bytes\n",
		kernel_size,
		kernel_size * 512
	);

	int err;
	if ((err = init(info)) != 0) {
		printf("encountered initialization error: 0x%x\n", err);
		asm(
			"cli\n"
			"hlt\n"
		);
	}

	raycaster_run();
	//run_shell();
	puts("\nThanks for using FrOSt\n");

	asm(
		"cli\n"
		"hlt\n"
	);
	return 0;
}

int init(multiboot_info* bootinfo) {
	int hal_err = hal_initialize(bootinfo);
	if (hal_err) return hal_err;

	register_timer_interrupt(scheduler_tick_handler);
	install_handlers();
	enable_interrupts();
	kbrd_install(33);

	// initialize physical memory
	uint64_t mem_size = get_memory_size_kb(bootinfo);
	void* kernel_end = (void*)(0x100000 + kernel_size * 512);
	struct mem_map memory_map = (struct mem_map) {
		bootinfo->mmap_length,
		(struct mmap_entry*) bootinfo->mmap_addr
	};
	pmem_init(mem_size, kernel_end, memory_map);
	pmem_init_region(
		(void*)0x100000,
		(kernel_size * 512 + PMEM_BLOCK_SIZE - 1) / PMEM_BLOCK_SIZE
	);

	vmem_init();

	//init_proclist();

	return 0;
}

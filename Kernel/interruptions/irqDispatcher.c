// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>
#include <stdint.h>

uint64_t int_20(uint64_t rsp);
static void int_21();
void keyboard_handler();

void irqDispatcher(uint64_t irq, uint64_t rsp) {
	switch (irq) {
		case 0:
			int_20(rsp);
			break;
		case 1:
			int_21();
			break;
	}
	return;
}

uint64_t int_20(uint64_t rsp) {
	return timer_handler(rsp);;
}

void int_21() {
	keyboard_handler();
}

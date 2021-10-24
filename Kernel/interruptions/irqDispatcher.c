#include <time.h>
#include <stdint.h>

static void int_20(uint64_t rsp);
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

void int_20(uint64_t rsp) {
	timer_handler(rsp);
}

void int_21() {
	keyboard_handler();
}

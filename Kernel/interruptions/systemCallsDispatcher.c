#include <stdint.h>
#include "systemCalls.h"
#include "memorymanager.h"

uint64_t systemCallsDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	switch (rdi) {
		case 0:
            return write(rsi, rdx, rcx);
        case 1:
            return read(rsi, rdx, rcx);
        case 2:
            return getTime(rsi, rdx, rcx);
        case 3:
            createProcess(rsi);
        case 4: 
            return (uint64_t) mallocMM(rsi);
        case 5:
            freeMM((void *) rsi);
            break;
        default:
            return -1;
	}
    return 1;
}
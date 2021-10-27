#include <stdint.h>
#include "systemCalls.h"
#include "memorymanager.h"
#include "pcb.h"

uint64_t systemCallsDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r10, uint64_t r9, uint64_t r8) {
	switch (rdi) {
		case 0:
            return write(rsi, rdx, rcx);
        case 1:
            return read(rsi, rdx, rcx);
        case 2:
            return getTime(rsi, rdx, rcx);
        case 3:
            createProcess((void*)rsi,rdx,(char**)rcx,r10,(int*)r9,(char*)r8);
            break;
        case 4: 
            return (uint64_t) mallocMM(rsi);
        case 5:
            freeMM((void *) rsi);
            break;
        case 6:
            psDisplay(); 
        default:
            return -1;
	}
    return 1;
}
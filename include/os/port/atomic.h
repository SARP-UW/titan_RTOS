/* global peterson lock */
#pragma once
#include <stdint.h>

struct os_atomic_shared_state {
  uint32_t interested[2];
  uint32_t turn;
};

/* By default the RTOS linker reserves this symbol in SRAM4
 * must be initialized to 0 */
extern struct os_atomic_shared_state os_atomic_shared_state;

/* Enter/exit must support nesting and restore the prior interrupt state.
 * Enter returns the current core (0 or 1); core_id returns that same core
 * until exit. NMI and fault handlers must not acquire the atomic lock. */
unsigned int os_atomic_port_enter(void);
unsigned int os_atomic_port_core_id(void);
void os_atomic_port_exit(void);
void os_atomic_port_fence(void);

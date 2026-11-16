/** Scheduler and platform interface required by the synchronization primitives. */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/*
 * All synchronization objects, threads using them, and the state protected by
 * them must be accessible and coherent on both cores. On STM32H745, the port
 * must arrange shared RAM and the M7 cache/MPU accordingly.
 *
 * lock/unlock serialize all calls on both cores, provide acquire/release memory
 * ordering for protected data, and prevent local preemption while held. The
 * lock must not be held while a thread is descheduled. None of the public
 * synchronization APIs may be called from an interrupt handler.
 */
void os_sync_port_lock(void);
void os_sync_port_unlock(void);

/* TODO: determine actual value to use here in thread API */
/* Until then, return a stable, non-NULL identity unique among live threads. */
const void *os_sync_port_current_thread(void);


uint64_t os_sync_port_now_ms(void);

/*
 * Called with the lock held. Atomically enroll the current thread, release
 * the lock and block; return with the lock held. UINT64_MAX means no timeout.
 * False indicates the deadline expired; true may be a spurious wakeup.
 * Infinite waits must not return false.
 */
bool os_sync_port_wait_until(uint64_t deadline);

/* Called with the lock held. Wake every waiter */
void os_sync_port_wake_all(void);

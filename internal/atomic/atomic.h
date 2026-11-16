#pragma once

/* Local interrupts/preemption remain disabled until the outermost exit.
 * Same-core nesting is supported; never sleep while this lock is held. */
void os_atomic_lock_enter(void);
void os_atomic_lock_exit(void);

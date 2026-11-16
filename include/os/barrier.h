
#pragma once
#include <stdbool.h>
#include <stdint.h>

// one shot
struct os_barrier_t { uint32_t parties; uint32_t arrived; uint32_t complete; };
#define OS_BARRIER_INIT(parties) { (parties), 0, 0 }
bool os_barrier_wait(struct os_barrier_t *barrier);

// reusable
struct os_reusable_barrier_t { uint32_t parties; uint32_t arrived; uint64_t generation; };
#define OS_REUSABLE_BARRIER_INIT(parties) { (parties), 0, 0 }
bool os_reusable_barrier_wait(struct os_reusable_barrier_t *barrier);

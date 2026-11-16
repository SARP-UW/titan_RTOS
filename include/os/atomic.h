/** Operations on shared 32-bit words, serialized by the RTOS atomic lock. */
#pragma once
#include <stdint.h>

uint32_t os_atomic_load(const volatile uint32_t *address);
void os_atomic_store(volatile uint32_t *address, uint32_t value);
uint32_t os_atomic_exchange(volatile uint32_t *address, uint32_t value);
uint32_t os_atomic_compare_exchange(volatile uint32_t *address, uint32_t expected,
                                    uint32_t desired);
uint32_t os_atomic_fetch_add(volatile uint32_t *address, uint32_t value);
uint32_t os_atomic_fetch_sub(volatile uint32_t *address, uint32_t value);
uint32_t os_atomic_fetch_and(volatile uint32_t *address, uint32_t value);
uint32_t os_atomic_fetch_or(volatile uint32_t *address, uint32_t value);
uint32_t os_atomic_fetch_xor(volatile uint32_t *address, uint32_t value);

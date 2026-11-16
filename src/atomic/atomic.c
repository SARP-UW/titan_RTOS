/**
 * This file is part of the Titan Project.
 * Copyright (c) 2026 UW SARP
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @file src/atomic/atomic.c
 * @authors Aaron McBride, Mahir Emran
 */
#include "os/atomic.h"
#include "os/port/atomic.h"
#include "internal/atomic/atomic.h"
#include "internal/system/sys_util.h"

_Static_assert(sizeof(struct os_atomic_shared_state) <= 32,
               "The atomic lock must fit in its reserved SRAM4 block");

static uint32_t lock_depth[2];

void os_atomic_lock_enter(void) {
  unsigned int core = os_atomic_port_enter();
  if (lock_depth[core]++ != 0) return;
  unsigned int other = core ^ 1U;
  __atomic_store_n(&os_atomic_shared_state.interested[core], 1U, __ATOMIC_RELAXED);
  __atomic_store_n(&os_atomic_shared_state.turn, other, __ATOMIC_RELAXED);
  os_atomic_port_fence();
  while (__atomic_load_n(&os_atomic_shared_state.interested[other], __ATOMIC_RELAXED) &&
         __atomic_load_n(&os_atomic_shared_state.turn, __ATOMIC_RELAXED) == other) {}
  os_atomic_port_fence();
}

void os_atomic_lock_exit(void) {
  unsigned int core = os_atomic_port_core_id();
  if (--lock_depth[core] == 0) {
    os_atomic_port_fence();
    __atomic_store_n(&os_atomic_shared_state.interested[core], 0U, __ATOMIC_RELAXED);
    os_atomic_port_fence();
  }
  os_atomic_port_exit();
}

uint32_t os_atomic_load(const volatile uint32_t *address) {
  os_atomic_lock_enter();
  uint32_t result = __atomic_load_n(address, __ATOMIC_RELAXED);
  os_atomic_lock_exit();
  return result;
}

void os_atomic_store(volatile uint32_t *address, uint32_t value) {
  os_atomic_lock_enter();
  __atomic_store_n(address, value, __ATOMIC_RELAXED);
  os_atomic_lock_exit();
}

uint32_t os_atomic_exchange(volatile uint32_t *address, uint32_t value) {
  os_atomic_lock_enter();
  uint32_t old = __atomic_load_n(address, __ATOMIC_RELAXED);
  __atomic_store_n(address, value, __ATOMIC_RELAXED);
  os_atomic_lock_exit();
  return old;
}

uint32_t os_atomic_compare_exchange(volatile uint32_t *address, uint32_t expected,
                                    uint32_t desired) {
  os_atomic_lock_enter();
  uint32_t old = __atomic_load_n(address, __ATOMIC_RELAXED);
  if (old == expected) __atomic_store_n(address, desired, __ATOMIC_RELAXED);
  os_atomic_lock_exit();
  return old;
}

uint32_t atomic_cmp_xchg(volatile uint32_t *address, uint32_t expected, uint32_t desired) {
  return os_atomic_compare_exchange(address, expected, desired);
}

#define DEFINE_FETCH_OPERATION(name, op) \
uint32_t os_atomic_fetch_##name(volatile uint32_t *address, uint32_t value) { \
  os_atomic_lock_enter(); \
  uint32_t old = __atomic_load_n(address, __ATOMIC_RELAXED); \
  __atomic_store_n(address, old op value, __ATOMIC_RELAXED); \
  os_atomic_lock_exit(); \
  return old; \
}

DEFINE_FETCH_OPERATION(add, +)
DEFINE_FETCH_OPERATION(sub, -)
DEFINE_FETCH_OPERATION(and, &)
DEFINE_FETCH_OPERATION(or, |)
DEFINE_FETCH_OPERATION(xor, ^)

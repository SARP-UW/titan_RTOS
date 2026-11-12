/**
 * This file is part of the Titan Project.
 * Copyright (c) 2025 UW SARP
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
 * @file os/src/internal/sys_util.c
 * @authors Aaron McBride
 * @brief TODO
 */

#include "internal/sys_util.h"
#include "internal/mmio.h"
#include "internal/os_assert.h"

enum core_id_t get_this_core_id(void) {
  static const uint32_t CM7_PARTNO = 0xC27;
  const uint32_t partno = read_field(SCB_CPUID, SCB_CPUID_PARTNO);
  return (partno == CM7_PARTNO) ? CORE_ID_CM7 : CORE_ID_CM4;
}

enum core_id_t get_other_core_id(void) {
  return (get_this_core_id() == CORE_ID_CM7) ? CORE_ID_CM4 : CORE_ID_CM7; 
}

uint32_t atomic_cmp_xchg(volatile uint32_t* ptr, uint32_t old, uint32_t new) {
  static volatile uint32_t lock_pending[2] = { 0 };
  static volatile uint32_t lock_owner = { 0 };
  const uint32_t this_core_num = (uint32_t)get_this_core_id();
  const uint32_t other_core_num = (uint32_t)get_other_core_id();
  uint32_t prev_value = 0;
  CRITICAL_SECTION(
    lock_pending[this_core_num] = 1;
    lock_owner = other_core_num;
    asm volatile ("dmb sy" ::: "memory");
    while ((lock_pending[other_core_num] != 0) && (lock_owner == other_core_num)) {}
    asm volatile ("dmb sy" ::: "memory");
    prev_value = *ptr;
    if (prev_value == old) {
      *ptr = new;
    }
    asm volatile ("dmb sy" ::: "memory");
    lock_pending[this_core_num] = 0;
  );
  return prev_value;
}

/* State information for critical sections. */
struct critical_state_t {
  int32_t depth;    /* Number of nested critical sections. */
  uint32_t primask; /* PRIMASK value before entering the critical section. */
};

/* Gets a pointer to the critical state struct for the current core. */
static struct critical_state_t* get_critical_state(void) {
  static struct critical_state_t critical_state_arr[2] = { 0 };
  return &critical_state_arr[get_this_core_id()];
}

void enter_critical(void) {
  uint32_t prev_primask = 0;
  asm volatile (
    "mrs %0, primask \n\t"
    "cpsid i         \n\t"
    : "= r" (prev_primask)
    :
    : "memory"
  );
  struct critical_state_t* const crit_state = get_critical_state();
  if (crit_state->depth == 0) {
    crit_state->primask = prev_primask;
  }
  crit_state->depth++;
}

void exit_critical(void) {
  struct critical_state_t* const crit_state = get_critical_state();
  if (os_assert(crit_state->depth > 0)) { 
    crit_state->depth--;
    if (crit_state->depth == 0) {
      asm volatile (
        "msr primask, %0 \n\t"
        :
        : "r" (crit_state->primask)
        : "memory"
      );
    }
  }
}



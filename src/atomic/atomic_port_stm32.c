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
 * @file src/atomic/atomic_port_stm32.c
 * @authors Mahir Emran
 */
#include "internal/system/sys_util.h"
#include "os/port/atomic.h"

unsigned int os_atomic_port_enter(void) {
  enter_critical();
  return (unsigned int)get_this_core_id();
}

unsigned int os_atomic_port_core_id(void) {
  return (unsigned int)get_this_core_id();
}

void os_atomic_port_exit(void) {
  exit_critical();
}

void os_atomic_port_fence(void) {
  asm volatile ("dmb sy" ::: "memory");
}

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
 * @file os/include/os/thread.h
 * @authors Aaron McBride
 * @brief TODO
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief TODO
 * @param entry
 * @param arg
 * @param stack
 */
void os_start(void (*entry)(void*), void* arg, void* stack);

/**
 * @brief TODO
 */
void os_exit(void);

/**
 * @brief TODO
 * @param X
 */
#define OS_ATOMIC(X) ({ \
  os_enter_atomic(); \
  do { X } while (0); \
  os_exit_atomic(); \
})

/** @internal */
void os_enter_atomic(void);
void os_exit_atomic(void);
/** @endinternal */
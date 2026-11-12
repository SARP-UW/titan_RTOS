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
 * @file os/include/os/mutex.h
 * @authors Aaron McBride
 * @brief TODO
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

/** @brief TODO */
struct os_mutex_t {
  uint8_t data[12];
};

/** @brief TODO */
#define OS_MUTEX_INIT { 0 }

/**
 * @brief TODO
 * @param mutex 
 * @param timeout 
 * @return 
 */
bool os_acquire(struct os_mutex_t* mutex, int64_t timeout);

/**
 * @brief TODO
 * @param mutex 
 */
void os_release(struct os_mutex_t* mutex);

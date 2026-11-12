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
 * @file os/include/os/semaphore.h
 * @authors Aaron McBride
 * @brief TODO
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

/** @brief TODO */
struct os_semaphore_t {
  uint8_t data[12];
};

/**
 * @brief TODO
 * @param count
 */
#define OS_SEMAPHORE_INIT(count) { 0 }

/**
 * @brief TODO
 * @param semaphore 
 * @param timeout 
 * @return 
 */
bool os_take(struct os_semaphore_t* semaphore, int64_t timeout);

/**
 * @brief TODO
 * @param semaphore
 * @return
 */
bool os_give(struct os_semaphore_t* semaphore);
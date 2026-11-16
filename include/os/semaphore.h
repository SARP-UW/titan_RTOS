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
 * Original author: Aaron McBride
 */

// Counting semaphore with a maximum count of UINT32_MAX
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "os/sync.h"

struct os_semaphore_t { uint32_t count; };
#define OS_SEMAPHORE_INIT(initial_count) { (initial_count) }

bool os_take(struct os_semaphore_t *semaphore, int64_t timeout);
bool os_give(struct os_semaphore_t *semaphore);

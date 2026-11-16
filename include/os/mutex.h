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

/** Non-recursive, thread-owned mutex. */
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "os/sync.h"

struct os_mutex_t { const void *owner; uint32_t locked; };
#define OS_MUTEX_INIT { NULL, 0 }

bool os_acquire(struct os_mutex_t *mutex, int64_t timeout);
bool os_release(struct os_mutex_t *mutex);

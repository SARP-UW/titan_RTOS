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
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "os/sync.h"

struct os_event_t { uint32_t raised; };
#define OS_EVENT_INIT { 0 }

bool os_await(struct os_event_t *event, int64_t timeout);
void os_raise(struct os_event_t *event);
void os_reset_event(struct os_event_t *event);

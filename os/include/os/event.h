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
 * @file os/include/os/event.h
 * @authors Aaron McBride
 * @brief TODO
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

/** @brief TODO */
struct os_event_t {
  uint8_t data[8];
};

/** @brief TODO */
#define OS_EVENT_INIT { 0 }

/**
 * @brief TODO
 * @param event 
 * @param timeout 
 * @return 
 */
bool os_await(struct os_event_t* event, int64_t timeout);

/**
 * @brief TODO
 * @param event 
 */
void os_raise(struct os_event_t* event);
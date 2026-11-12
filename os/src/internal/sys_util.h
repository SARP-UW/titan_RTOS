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
 * @file os/src/internal/sys_util.h
 * @authors Aaron McBride
 * @brief TODO
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

/** @brief Identifies distinct processor cores. */
enum core_id_t {
  CORE_ID_CM7 = 0, /** Cortex-M7 core */
  CORE_ID_CM4 = 1, /** Cortex-M4 core */
};

/**
 * @brief TODO
 * @returns
 */
enum core_id_t get_this_core_id(void);

/**
 * @brief TODO
 * @returns 
 */
enum core_id_t get_other_core_id(void);

/**
 * @brief
 * @param ptr (volatile uint32_t*)
 * @param old (uint32_t)
 * @param new (uint32_t)
 * @returns (uint32_t)
 */
uint32_t atomic_cmp_xchg(volatile uint32_t* ptr, uint32_t old, uint32_t new);

/**
 * @brief TODO
 * @param X
 * @note
 */
/** @internal *************************************************************************************/

void enter_critical(void);
void exit_critical(void);

#define CRITICAL_SECTION(X) ({ \
  enter_critical(); \
  X \
  exit_critical(); \
})

/** @endinternal **********************************************************************************/

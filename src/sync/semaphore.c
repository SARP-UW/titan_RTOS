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
 * @file src/sync/semaphore.c
 * @authors Aaron McBride, Mahir Emran
 */

#include "os/semaphore.h"
#include "os/atomic.h"
#include "internal/sync/sync.h"

bool os_take(struct os_semaphore_t *semaphore, int64_t timeout) {
  uint64_t deadline;
  if (!semaphore || !sync_deadline(timeout, &deadline)) return false;
  os_sync_port_lock();
  while (!os_atomic_load(&semaphore->count)) {
    if (!sync_wait(deadline)) {
      if (!os_atomic_load(&semaphore->count)) {
        os_sync_port_unlock();
        return false;
      }
      break;
    }
  }
  os_atomic_fetch_sub(&semaphore->count, 1);
  os_sync_port_unlock();
  return true;
}

bool os_give(struct os_semaphore_t *semaphore) {
  if (!semaphore) return false;
  os_sync_port_lock();
  bool ok = os_atomic_load(&semaphore->count) != UINT32_MAX;
  if (ok) {
    os_atomic_fetch_add(&semaphore->count, 1);
    os_sync_port_wake_all();
  }
  os_sync_port_unlock();
  return ok;
}

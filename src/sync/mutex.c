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
 * @file src/sync/mutex.c
 * @authors Aaron McBride, Mahir Emran
 */

#include "os/mutex.h"
#include "os/atomic.h"
#include "internal/sync/sync.h"
#include <stddef.h>

bool os_acquire(struct os_mutex_t *mutex, int64_t timeout) {
  uint64_t deadline;
  if (!mutex || !sync_deadline(timeout, &deadline))
    return false;
  const void *thread = os_sync_port_current_thread();
  if (!thread)
    return false;
  os_sync_port_lock();
  while (os_atomic_load(&mutex->locked)) {
    if (mutex->owner == thread || !sync_wait(deadline)) {
      os_sync_port_unlock();
      return false;
    }
  }
  mutex->owner = thread;
  os_atomic_store(&mutex->locked, 1);
  os_sync_port_unlock();
  return true;
}

bool os_release(struct os_mutex_t *mutex) {
  if (!mutex)
    return false;
  const void *thread = os_sync_port_current_thread();
  if (!thread)
    return false;
  os_sync_port_lock();
  bool owned = os_atomic_load(&mutex->locked) && mutex->owner == thread;
  if (owned) {
    mutex->owner = NULL;
    os_atomic_store(&mutex->locked, 0);
    os_sync_port_wake_all();
  }
  os_sync_port_unlock();
  return owned;
}

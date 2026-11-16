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
 * @file src/sync/barrier.c
 * @authors Mahir Emran
 */
#include "os/barrier.h"
#include "os/atomic.h"
#include "internal/sync/sync.h"

bool os_barrier_wait(struct os_barrier_t *barrier) {
  if (!barrier)
    return false;
  os_sync_port_lock();
  if (!barrier->parties) {
    os_sync_port_unlock();
    return false;
  }
  if (!os_atomic_load(&barrier->complete) &&
      os_atomic_fetch_add(&barrier->arrived, 1) + 1 == barrier->parties) {
    os_atomic_store(&barrier->complete, 1);
    os_sync_port_wake_all();
  }
  while (!os_atomic_load(&barrier->complete))
    os_sync_port_wait_until(UINT64_MAX);
  os_sync_port_unlock();
  return true;
}

bool os_reusable_barrier_wait(struct os_reusable_barrier_t *barrier) {
  if (!barrier)
    return false;
  os_sync_port_lock();
  if (!barrier->parties) {
    os_sync_port_unlock();
    return false;
  }
  uint64_t generation = barrier->generation;
  if (os_atomic_fetch_add(&barrier->arrived, 1) + 1 == barrier->parties) {
    os_atomic_store(&barrier->arrived, 0);
    ++barrier->generation;
    os_sync_port_wake_all();
  } else {
    while (generation == barrier->generation)
      os_sync_port_wait_until(UINT64_MAX);
  }
  os_sync_port_unlock();
  return true;
}

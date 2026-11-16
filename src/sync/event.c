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
 * @file src/sync/event.c
 * @authors Aaron McBride, Mahir Emran
 */

#include "os/event.h"
#include "os/atomic.h"
#include "internal/sync/sync.h"

bool os_await(struct os_event_t *event, int64_t timeout) {
  uint64_t deadline;
  if (!event || !sync_deadline(timeout, &deadline)) return false;
  os_sync_port_lock();
  while (!os_atomic_load(&event->raised)) {
    if (!sync_wait(deadline)) {
      bool raised = os_atomic_load(&event->raised) != 0;
      os_sync_port_unlock();
      return raised;
    }
  }
  os_sync_port_unlock();
  return true;
}

void os_raise(struct os_event_t *event) {
  if (!event) return;
  os_sync_port_lock();
  os_atomic_store(&event->raised, 1);
  os_sync_port_wake_all();
  os_sync_port_unlock();
}

void os_reset_event(struct os_event_t *event) {
  if (!event) return;
  os_sync_port_lock();
  os_atomic_store(&event->raised, 0);
  os_sync_port_unlock();
}

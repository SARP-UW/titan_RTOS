#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "os/sync.h"
#include "os/port/sync.h"

static inline bool sync_deadline(int64_t timeout, uint64_t *deadline) {
  if (timeout < OS_WAIT_FOREVER) return false;
  if (timeout == OS_WAIT_FOREVER) {
    *deadline = UINT64_MAX;
  } else {
    uint64_t now = os_sync_port_now_ms();
    uint64_t duration = (uint64_t)timeout;
    *deadline = duration >= UINT64_MAX - now ? UINT64_MAX - 1 : now + duration;
  }
  return true;
}

static inline bool sync_wait(uint64_t deadline) {
  if (deadline != UINT64_MAX && os_sync_port_now_ms() >= deadline) return false;
  return os_sync_port_wait_until(deadline);
}

#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "os/port/sync.h"

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static _Thread_local int identity;
static unsigned int waiters;

void os_sync_port_lock(void) { pthread_mutex_lock(&lock); }
void os_sync_port_unlock(void) { pthread_mutex_unlock(&lock); }
const void *os_sync_port_current_thread(void) { return &identity; }
uint64_t os_sync_port_now_ms(void) {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return (uint64_t)now.tv_sec * 1000 + (uint64_t)now.tv_nsec / 1000000;
}
bool os_sync_port_wait_until(uint64_t deadline) {
  ++waiters;
  int rc;
  if (deadline == UINT64_MAX) {
    rc = pthread_cond_wait(&cond, &lock);
  } else {
    uint64_t now = os_sync_port_now_ms();
    uint64_t remaining = deadline > now ? deadline - now : 0;
    if (remaining > 1000) remaining = 1000;
    struct timespec until;
    clock_gettime(CLOCK_REALTIME, &until);
    until.tv_sec += (time_t)(remaining / 1000);
    until.tv_nsec += (long)(remaining % 1000) * 1000000;
    if (until.tv_nsec >= 1000000000) { ++until.tv_sec; until.tv_nsec -= 1000000000; }
    rc = pthread_cond_timedwait(&cond, &lock, &until);
  }
  --waiters;
  if (rc != 0 && rc != ETIMEDOUT) abort();
  return rc == 0 || os_sync_port_now_ms() < deadline;
}
void os_sync_port_wake_all(void) { pthread_cond_broadcast(&cond); }
unsigned int host_waiters(void) {
  pthread_mutex_lock(&lock);
  unsigned int value = waiters;
  pthread_mutex_unlock(&lock);
  return value;
}

#include <pthread.h>
#include <stdlib.h>
#include "os/port/atomic.h"

struct os_atomic_shared_state os_atomic_shared_state = { { 0, 0 }, 0 };
static pthread_mutex_t core_locks[2] = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER };
static _Thread_local unsigned int core_id;
static _Thread_local unsigned int depth;

void host_atomic_set_core(unsigned int core) {
  if (core > 1 || depth) abort();
  core_id = core;
}
unsigned int os_atomic_port_enter(void) {
  if (depth++ == 0 && pthread_mutex_lock(&core_locks[core_id])) abort();
  return core_id;
}
unsigned int os_atomic_port_core_id(void) { return core_id; }
void os_atomic_port_exit(void) {
  if (!depth) abort();
  if (--depth == 0 && pthread_mutex_unlock(&core_locks[core_id])) abort();
}
void os_atomic_port_fence(void) { __atomic_thread_fence(__ATOMIC_SEQ_CST); }

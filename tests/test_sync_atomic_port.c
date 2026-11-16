#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "internal/atomic/atomic.h"
#include "os/barrier.h"
#include "os/event.h"
#include "os/mutex.h"
#include "os/port/sync.h"
#include "os/semaphore.h"

#define CHECK(expr) do { if (!(expr)) { \
  fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #expr); exit(1); \
} } while (0)

static int thread_id;
void os_sync_port_lock(void) { os_atomic_lock_enter(); }
void os_sync_port_unlock(void) { os_atomic_lock_exit(); }
const void *os_sync_port_current_thread(void) { return &thread_id; }
uint64_t os_sync_port_now_ms(void) { return 0; }
bool os_sync_port_wait_until(uint64_t deadline) { (void)deadline; abort(); }
void os_sync_port_wake_all(void) {}

int main(void) {
  struct os_event_t event = OS_EVENT_INIT;
  struct os_mutex_t mutex = OS_MUTEX_INIT;
  struct os_semaphore_t semaphore = OS_SEMAPHORE_INIT(1);
  struct os_barrier_t barrier = OS_BARRIER_INIT(1);
  struct os_reusable_barrier_t reusable = OS_REUSABLE_BARRIER_INIT(1);
  CHECK(!os_await(&event, OS_NO_WAIT));
  os_raise(&event);
  CHECK(os_await(&event, OS_NO_WAIT));
  os_reset_event(&event);
  CHECK(os_acquire(&mutex, OS_NO_WAIT));
  CHECK(!os_acquire(&mutex, OS_NO_WAIT));
  CHECK(os_release(&mutex));
  CHECK(os_take(&semaphore, OS_NO_WAIT));
  CHECK(!os_take(&semaphore, OS_NO_WAIT));
  CHECK(os_give(&semaphore));
  CHECK(os_barrier_wait(&barrier));
  CHECK(os_reusable_barrier_wait(&reusable));
  CHECK(os_reusable_barrier_wait(&reusable));
  return 0;
}

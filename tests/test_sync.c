#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "os/barrier.h"
#include "os/event.h"
#include "os/mutex.h"
#include "os/semaphore.h"
#include "os/port/sync.h"

#define CHECK(expr) do { if (!(expr)) { \
  fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #expr); exit(1); \
} } while (0)

unsigned int host_waiters(void);
static void wait_for_waiters(unsigned int expected) {
  uint64_t until = os_sync_port_now_ms() + 3000;
  while (host_waiters() < expected) {
    CHECK(os_sync_port_now_ms() < until);
    struct timespec pause = { .tv_nsec = 1000000 };
    nanosleep(&pause, NULL);
  }
}

static struct os_event_t event = OS_EVENT_INIT;
static struct os_mutex_t mutex = OS_MUTEX_INIT;
static struct os_semaphore_t semaphore = OS_SEMAPHORE_INIT(2);
static struct os_barrier_t barrier = OS_BARRIER_INIT(3);
static struct os_reusable_barrier_t reusable = OS_REUSABLE_BARRIER_INIT(3);
static unsigned int arrivals[200];

static void *event_waiter(void *unused) {
  (void)unused;
  CHECK(os_await(&event, OS_WAIT_FOREVER));
  return NULL;
}
static void *mutex_other(void *unused) {
  (void)unused;
  CHECK(!os_release(&mutex));
  CHECK(!os_acquire(&mutex, OS_NO_WAIT));
  CHECK(!os_acquire(&mutex, 20));
  os_raise(&event);
  CHECK(os_acquire(&mutex, 2000));
  CHECK(os_release(&mutex));
  return NULL;
}
static void *semaphore_waiter(void *unused) {
  (void)unused;
  CHECK(os_take(&semaphore, 2000));
  return NULL;
}
static void *barrier_waiter(void *unused) {
  (void)unused;
  CHECK(os_barrier_wait(&barrier));
  for (unsigned int i = 0; i < 200; ++i) {
    os_sync_port_lock();
    ++arrivals[i];
    os_sync_port_unlock();
    CHECK(os_reusable_barrier_wait(&reusable));
    os_sync_port_lock();
    CHECK(arrivals[i] == 3);
    os_sync_port_unlock();
  }
  return NULL;
}

int main(void) {
  CHECK(!os_await(&event, OS_NO_WAIT));
  CHECK(!os_await(&event, -2));
  uint64_t before = os_sync_port_now_ms();
  CHECK(!os_await(&event, 30));
  CHECK(os_sync_port_now_ms() - before >= 20);
  pthread_t a, b;
  CHECK(!pthread_create(&a, NULL, event_waiter, NULL));
  CHECK(!pthread_create(&b, NULL, event_waiter, NULL));
  wait_for_waiters(2);
  os_raise(&event);
  CHECK(!pthread_join(a, NULL));
  CHECK(!pthread_join(b, NULL));
  CHECK(os_await(&event, OS_NO_WAIT));
  os_reset_event(&event);
  CHECK(!os_await(&event, OS_NO_WAIT));

  CHECK(os_take(&semaphore, OS_NO_WAIT));
  CHECK(os_take(&semaphore, OS_NO_WAIT));
  CHECK(!os_take(&semaphore, OS_NO_WAIT));
  CHECK(os_give(&semaphore));
  CHECK(os_take(&semaphore, OS_NO_WAIT));
  CHECK(!os_take(&semaphore, 20));
  CHECK(!pthread_create(&a, NULL, semaphore_waiter, NULL));
  wait_for_waiters(1);
  CHECK(os_give(&semaphore));
  CHECK(!pthread_join(a, NULL));
  CHECK(!os_take(&semaphore, OS_NO_WAIT));
  struct os_semaphore_t full = OS_SEMAPHORE_INIT(UINT32_MAX);
  CHECK(!os_give(&full));
  CHECK(full.count == UINT32_MAX);

  CHECK(os_acquire(&mutex, OS_NO_WAIT));
  CHECK(!os_acquire(&mutex, OS_NO_WAIT));
  CHECK(!pthread_create(&a, NULL, mutex_other, NULL));
  CHECK(os_await(&event, 2000));
  wait_for_waiters(1);
  CHECK(os_release(&mutex));
  CHECK(!pthread_join(a, NULL));
  CHECK(!os_release(&mutex));

  struct os_barrier_t invalid = OS_BARRIER_INIT(0);
  struct os_reusable_barrier_t invalid_reusable = OS_REUSABLE_BARRIER_INIT(0);
  CHECK(!os_barrier_wait(&invalid));
  CHECK(!os_reusable_barrier_wait(&invalid_reusable));
  CHECK(!pthread_create(&a, NULL, barrier_waiter, NULL));
  CHECK(!pthread_create(&b, NULL, barrier_waiter, NULL));
  wait_for_waiters(2);
  barrier_waiter(NULL);
  CHECK(!pthread_join(a, NULL));
  CHECK(!pthread_join(b, NULL));
  CHECK(os_barrier_wait(&barrier));
  return 0;
}

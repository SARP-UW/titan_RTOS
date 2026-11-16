#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "os/atomic.h"
#include "internal/atomic/atomic.h"

#define CHECK(expr) do { if (!(expr)) { \
  fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #expr); exit(1); \
} } while (0)

void host_atomic_set_core(unsigned int core);
static uint32_t counter;
static uint32_t nested_counter;
static unsigned int in_section;
static unsigned int ready;

static void *worker(void *arg) {
  host_atomic_set_core((unsigned int)(uintptr_t)arg);
  __atomic_fetch_add(&ready, 1, __ATOMIC_SEQ_CST);
  while (__atomic_load_n(&ready, __ATOMIC_SEQ_CST) != 2) {}
  for (unsigned int i = 0; i < 20000; ++i) {
    os_atomic_fetch_add(&counter, 1);
    os_atomic_lock_enter();
    CHECK(__atomic_fetch_add(&in_section, 1, __ATOMIC_RELAXED) == 0);
    os_atomic_fetch_add(&nested_counter, 1);
    CHECK(__atomic_fetch_sub(&in_section, 1, __ATOMIC_RELAXED) == 1);
    os_atomic_lock_exit();
  }
  return NULL;
}

int main(void) {
  uint32_t word = 0;
  CHECK(os_atomic_load(&word) == 0);
  os_atomic_store(&word, 4);
  CHECK(os_atomic_exchange(&word, 7) == 4);
  CHECK(os_atomic_compare_exchange(&word, 4, 9) == 7);
  CHECK(os_atomic_load(&word) == 7);
  CHECK(os_atomic_compare_exchange(&word, 7, 9) == 7);
  CHECK(os_atomic_fetch_add(&word, 3) == 9);
  CHECK(os_atomic_fetch_sub(&word, 4) == 12);
  CHECK(os_atomic_fetch_or(&word, 3) == 8);
  CHECK(os_atomic_fetch_and(&word, 6) == 11);
  CHECK(os_atomic_fetch_xor(&word, 7) == 2);
  CHECK(os_atomic_load(&word) == 5);
  os_atomic_store(&word, UINT32_MAX);
  CHECK(os_atomic_fetch_add(&word, 1) == UINT32_MAX);
  CHECK(os_atomic_fetch_sub(&word, 1) == 0);
  CHECK(os_atomic_load(&word) == UINT32_MAX);

  pthread_t first, second;
  CHECK(!pthread_create(&first, NULL, worker, (void *)(uintptr_t)0));
  CHECK(!pthread_create(&second, NULL, worker, (void *)(uintptr_t)1));
  CHECK(!pthread_join(first, NULL));
  CHECK(!pthread_join(second, NULL));
  CHECK(os_atomic_load(&counter) == 40000);
  CHECK(os_atomic_load(&nested_counter) == 40000);
  return 0;
}

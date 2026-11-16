

/// CORE 1 -> calls scheduler
// acquire lock
/// CORE 2 -> calls scheduler
// acquire lock -> but is blocked
// finds next thread for core 1
// returns next thread for core 1
// core 1 releases the lock
// core 2 acquires the lock
// core 2 finds its thread.

struct thread {
  int state;
};

void* scheduler() {
  acquire_lock();
  void* next_thread = find_next_thread();
  release_lock();
  return next_thread;
}
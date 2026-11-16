# Titan RTOS

Titan RTOS is an early-stage project to develop a dual-core real-time operating
system for the [Titan flight computer](https://github.com/SARP-UW/titan). The
initial hardware target is the STM32H745XIH3, which has Cortex-M7 and Cortex-M4
cores. The RTOS lives in its own repository so it can be developed separately
and eventually linked into Titan.

## Project status

This repository is still being set up. The existing source contains preliminary
utilities, synchronization primitives, and API stubs; it is not yet a working
RTOS. Building the library does not provide a scheduler, a bootable image, or a
tested dual-core system. The synchronization primitives require a scheduler and
platform port before they can be used on the STM32.

The intended scope includes threads, scheduling, synchronization, and time and
system utilities. The original [design notes](docs/design-notes.md) capture
ideas and open questions, not settled implementation decisions.

## Future work

- Define the threading and scheduling model, then implement thread management
  and context switching for both cores.
- Implement the synchronization port for the scheduler and STM32, including
  cross-core coordination, shared memory, and the M7 cache.
- Add timekeeping, sleeping, and system utilities.
- Establish the startup, interrupt, vector-table, and memory-layout requirements
  for applications using the RTOS.
- Expand host tests and add board tests for timing, interrupts, context
  switching, and dual-core behavior.
- Integrate the library with Titan and validate it on the target hardware.

## Repository layout

- `include/os/` — draft public interfaces.
- `src/` — implementation, grouped by component.
- `internal/` — private headers, grouped to match the source.
- `cmake/` — standalone Arm cross-compilation toolchain.
- `docs/` — working design notes.
- `tests/` — host tests and a test-only synchronization port.

## Building

Install [CMake](https://cmake.org/download/) 3.19 or newer and the
[Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
for the `arm-none-eabi` target. Make sure `arm-none-eabi-gcc` is on your `PATH`.
A native build tool such as Make or Ninja is also required.

The build script builds both core libraries and never flashes hardware:

```sh
./build.sh all   # build both libraries and run host tests when available
./build.sh test  # run host tests when available
```

The `all` command requires Arm GCC on `PATH`. Running `./build.sh` without an
argument is the same as `./build.sh all`.

Build the static library for the Cortex-M7 (the default):

```sh
cmake -S . -B build-m7 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build-m7
```

For the Cortex-M4, use a separate build directory:

```sh
cmake -S . -B build-m4 -DTITAN_RTOS_CORE=m4 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build-m4
```

Each build produces `libtitan_rtos.a`. The configuration follows Titan's
Thumb, single-precision FPU, and hard-float settings. Build the library separately
for each core; the two cores need their own firmware images. This setup does not
flash a board or provide startup code, vector tables, or a linker script.

## Testing

`./build.sh test` builds and runs the synchronization tests with a native C
compiler and POSIX threads in `build-host/`, independently of the Arm
cross-builds. The host port is for testing and is not an STM32 implementation.

As the RTOS grows, scheduling decisions, queues, time calculations, and other
hardware-independent logic can be tested on the host, with fakes where needed.
The STM32 will still be needed to validate startup, interrupts, context
switches, actual peripheral behavior, timing, and cross-core synchronization,
including memory ordering and cache effects. Host tests alone cannot establish
that those behaviors work on the board.

## Synchronization primitives

The library provides a manual-reset, level-triggered event (`os_await`,
`os_raise`, `os_reset_event`), a non-recursive thread-owned mutex (`os_acquire`,
`os_release`), a counting semaphore (`os_take`, `os_give`), and one-shot and
reusable barriers (`os_barrier_wait`, `os_reusable_barrier_wait`). Initializer
macros and operation details are in their respective headers under `include/os/`.

Event, mutex, and semaphore timeouts are in milliseconds: `OS_NO_WAIT` polls,
`OS_WAIT_FOREVER` waits indefinitely, and positive values give a relative
timeout. Barriers wait indefinitely. A raised event stays raised until reset;
resetting it before a woken thread reacquires the lock can cause that thread to
wait again. Barriers require a nonzero participant count, and participants must
call the reusable barrier once per round. No fairness or priority inheritance is
currently guaranteed. Objects must not be reinitialized while in use, and a
thread must release its mutexes before exiting.

These APIs are for thread context, not interrupt handlers. They require the
functions declared in `os/port/sync.h`: globally unique current-thread identity,
a shared monotonic clock, an internal lock, and atomic scheduler wait/wake
operations. No STM32 port or scheduler implementation is supplied yet; linking
an application that uses these primitives therefore requires those functions.
The host tests use a POSIX port solely to exercise the portable logic.

On STM32H745, synchronization objects and protected shared data need to reside
in memory accessible and coherent to both cores. The port must supply the
necessary cross-core exclusion, interrupt/preemption handling, memory ordering,
and M7 cache/MPU configuration. See ST's [inter-processor communication guide
(AN5617)](https://www.st.com/resource/en/application_note/an5617-stm32h745755-and-stm32h747757-lines-interprocessor-communications-stmicroelectronics.pdf).

### Atomic operations and Peterson's lock

`os/atomic.h` provides 32-bit load, store, exchange, compare-exchange, and
fetch-and-modify operations. Their implementation in `src/atomic/atomic.c` uses one
global Peterson lock; the old `atomic_cmp_xchg` delegates to the
same implementation. Arithmetic wraps as unsigned 32-bit arithmetic. Every
concurrent access to a protected word must use this atomic API, and the word
must be naturally aligned and coherent to both cores.

By default, linking `titan_rtos::titan_rtos` adds the linker fragment
`cmake/stm32h745-shared-sram4.ld`. It reserves 32 bytes at
`0x3800ffe0–0x3800ffff`, at the end of SRAM4, and defines
`os_atomic_shared_state` there. The application linker script must define the
full 64 KiB `SRAM4` memory region; overlapping allocations cause a link error.
Use the fragment in **both** core images. To manage the reservation in a custom
linker script, set `TITAN_RTOS_RESERVE_SRAM4=OFF` and define the symbol at the
same coherent shared-RAM address in both images.

The reserved section is not initialized automatically. Before either core uses
atomics, startup must initialize the state to zero exactly once and coordinate
both cores so the other core cannot access it early or clear it again. Configure
the M7 MPU/cache for coherence (for example, make the region non-cacheable).
The reservation alone does not perform startup coordination or MPU setup.
These operations have not yet been validated on hardware.

The STM32 atomic port disables local interrupts while taking the lock and uses
memory barriers; NMI and fault handlers must not use it. Same-core nesting is
supported, but the lock must never be held across a blocking operation. The
synchronization primitives use atomic operations for their 32-bit state and the
scheduler port's lock to coordinate that state with wait/wake operations. The
mutex owner and reusable barrier generation are protected by the scheduler lock.
The future scheduler port can use the Peterson lock for this handshake, provided
it enrolls the waiter and releases the lock atomically before blocking. If it
uses a separate lock, code must acquire that lock before the Peterson lock and
must not acquire it while already holding the Peterson lock.

## Using the library from another CMake project

Once the RTOS is ready for integration, a cross-compiled firmware project can
add a local checkout and link the target:

```cmake
# Configure the parent project for the same core and Arm GCC toolchain first.
set(TITAN_RTOS_CORE m7 CACHE STRING "RTOS target core")
add_subdirectory(path/to/titan_RTOS)
target_link_libraries(your_firmware PRIVATE titan_rtos::titan_rtos)
```

Replace the path and target name with those in the parent project. The library
exports its public include directory and CPU/ABI flags to consumers. The parent
must use matching core and ABI settings and provide its own startup, memory
layout, and linker script. Integration with Titan is planned and has not yet
been implemented or tested in Titan itself.

## References

- [STM32H745xI/G datasheet](https://www.st.com/resource/en/datasheet/stm32h745xi.pdf) — device features, memory, and electrical specifications.
- [STM32H745/755 and STM32H747/757 reference manual (RM0399)](https://www.st.com/resource/en/reference_manual/dm00176879-stm32h745-755-and-stm32h747-757-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf) — peripherals, memory, and device behavior.
- [STM32H745/755 documentation](https://www.st.com/en/microcontrollers-microprocessors/stm32h745-755/documentation.html) — manuals and device errata.
- [Armv7-M Architecture Reference Manual](https://static.docs.arm.com/ddi0403/ed/DDI0403E_d_armv7m_arm.pdf) — architecture shared by Cortex-M4 and Cortex-M7.
- [Cortex-M7 programming manual (PM0253)](https://www.st.com/resource/en/programming_manual/DM00237416.pdf) and [Cortex-M4 programming manual (PM0214)](https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf) — processor programming models and core peripherals.
- [Cortex-M RTOS context switching](https://interrupt.memfault.com/blog/cortex-m-rtos-context-switching) — a practical introduction to context switches.

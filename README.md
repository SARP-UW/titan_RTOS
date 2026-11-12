# Titan RTOS

Titan RTOS is an early-stage project to develop a dual-core real-time operating
system for the [Titan flight computer](https://github.com/SARP-UW/titan). The
initial hardware target is the STM32H745XIH3, which has Cortex-M7 and Cortex-M4
cores. The RTOS lives in its own repository so it can be developed separately
and eventually linked into Titan.

## Project status

This repository is still being set up. The existing source contains preliminary
utilities and API stubs; it is not yet a working RTOS. Building the library does
not provide a scheduler, working synchronization primitives, a bootable image,
or a tested dual-core system.

The intended scope includes threads, scheduling, synchronization, and time and
system utilities. The original [design notes](docs/design-notes.md) capture
ideas and open questions, not settled implementation decisions.

## Future work

- Define the threading and scheduling model, then implement thread management
  and context switching for both cores.
- Design and implement synchronization primitives and cross-core coordination,
  accounting for interrupts, shared memory, and the M7 cache.
- Add timekeeping, sleeping, and system utilities.
- Establish the startup, interrupt, vector-table, and memory-layout requirements
  for applications using the RTOS.
- Add host tests for hardware-independent logic and board tests for timing,
  interrupts, context switching, and dual-core behavior.
- Integrate the library with Titan and validate it on the target hardware.

## Repository layout

- `os/include/os/` — draft public interfaces.
- `os/src/` — preliminary source and internal utilities.
- `cmake/` — standalone Arm cross-compilation toolchain.
- `docs/` — working design notes.

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

There are no tests configured yet. `./build.sh test` reports this until a host
test suite is added. Once configured, host tests will run independently of the
Arm cross-builds in `build-host/` and require a native C compiler.

As the RTOS grows, scheduling decisions, queues, time calculations, and other
hardware-independent logic can be tested on the host, with fakes where needed.
The STM32 will still be needed to validate startup, interrupts, context
switches, actual peripheral behavior, timing, and cross-core synchronization,
including memory ordering and cache effects. Host tests alone cannot establish
that those behaviors work on the board.

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

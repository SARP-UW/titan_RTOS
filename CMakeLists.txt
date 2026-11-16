cmake_minimum_required(VERSION 3.19)

# Select the cross compiler only for standalone builds; a parent project owns
# its toolchain when this directory is added with add_subdirectory().
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR AND NOT CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-none-eabi.cmake")
endif()

project(titan_rtos LANGUAGES C)

set(TITAN_RTOS_CORE "m7" CACHE STRING "Target core: m7 or m4")
set_property(CACHE TITAN_RTOS_CORE PROPERTY STRINGS m7 m4)
if(TITAN_RTOS_CORE STREQUAL "m7")
  set(titan_rtos_cpu cortex-m7)
elseif(TITAN_RTOS_CORE STREQUAL "m4")
  set(titan_rtos_cpu cortex-m4)
else()
  message(FATAL_ERROR "TITAN_RTOS_CORE must be m7 or m4")
endif()

add_library(titan_rtos STATIC
  src/atomic/atomic.c
  src/atomic/atomic_port_stm32.c
  src/sync/barrier.c
  src/sync/event.c
  src/sync/mutex.c
  src/sync/semaphore.c
  src/system/debug.c
  src/system/error.c
  src/system/sys.c
  src/system/lang_util.c
  src/system/mmio.c
  src/system/sys_util.c
  src/thread/thread.c
  src/time/time.c
)
add_library(titan_rtos::titan_rtos ALIAS titan_rtos)

option(TITAN_RTOS_RESERVE_SRAM4 "Reserve SRAM4 for the shared atomic lock" ON)
if(TITAN_RTOS_RESERVE_SRAM4)
  set(titan_rtos_shared_linker_script "${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32h745-shared-sram4.ld")
  target_link_options(titan_rtos INTERFACE "LINKER:-T,${titan_rtos_shared_linker_script}")
  set_property(TARGET titan_rtos APPEND PROPERTY INTERFACE_LINK_DEPENDS "${titan_rtos_shared_linker_script}")
endif()

target_include_directories(titan_rtos
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_compile_features(titan_rtos PUBLIC c_std_17)
set_target_properties(titan_rtos PROPERTIES C_EXTENSIONS ON)

# Match Titan's single-precision hard-float ABI on both cores.
target_compile_options(titan_rtos PUBLIC
  -mcpu=${titan_rtos_cpu} -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
)
target_compile_options(titan_rtos PRIVATE -ffreestanding -fdata-sections)
target_link_options(titan_rtos INTERFACE
  -mcpu=${titan_rtos_cpu} -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
)

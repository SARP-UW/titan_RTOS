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
  os/src/debug.c
  os/src/error.c
  os/src/event.c
  os/src/mutex.c
  os/src/semaphore.c
  os/src/sys.c
  os/src/thread.c
  os/src/time.c
  os/src/internal/lang_util.c
  os/src/internal/mmio.c
  os/src/internal/sys_util.c
)
add_library(titan_rtos::titan_rtos ALIAS titan_rtos)

target_include_directories(titan_rtos
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/os/include"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/os/src"
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

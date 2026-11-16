cmake_minimum_required(VERSION 3.19)
project(titan_rtos_sync_tests C)
find_package(Threads REQUIRED)
enable_testing()
add_executable(test_sync
  test_sync.c host_sync_port.c host_atomic_port.c
  ../src/atomic/atomic.c
  ../src/sync/event.c ../src/sync/mutex.c ../src/sync/semaphore.c ../src/sync/barrier.c)
target_include_directories(test_sync PRIVATE ../include ..)
target_compile_features(test_sync PRIVATE c_std_17)
target_link_libraries(test_sync PRIVATE Threads::Threads)
add_test(NAME synchronization COMMAND test_sync)
set_tests_properties(synchronization PROPERTIES TIMEOUT 20)

add_executable(test_atomic test_atomic.c host_atomic_port.c ../src/atomic/atomic.c)
target_include_directories(test_atomic PRIVATE ../include ..)
target_compile_features(test_atomic PRIVATE c_std_17)
target_link_libraries(test_atomic PRIVATE Threads::Threads)
add_test(NAME atomic_operations COMMAND test_atomic)
set_tests_properties(atomic_operations PROPERTIES TIMEOUT 20)

add_executable(test_sync_atomic_port
  test_sync_atomic_port.c host_atomic_port.c ../src/atomic/atomic.c
  ../src/sync/event.c ../src/sync/mutex.c ../src/sync/semaphore.c ../src/sync/barrier.c)
target_include_directories(test_sync_atomic_port PRIVATE ../include ..)
target_compile_features(test_sync_atomic_port PRIVATE c_std_17)
target_link_libraries(test_sync_atomic_port PRIVATE Threads::Threads)
add_test(NAME synchronization_shared_atomic_lock COMMAND test_sync_atomic_port)
set_tests_properties(synchronization_shared_atomic_lock PROPERTIES TIMEOUT 20)

find_program(ARM_GCC arm-none-eabi-gcc)
find_program(ARM_NM arm-none-eabi-nm)
find_program(ARM_READELF arm-none-eabi-readelf)
if(ARM_GCC AND ARM_NM AND ARM_READELF)
  add_test(NAME shared_sram4_link
    COMMAND ${CMAKE_COMMAND}
      -DARM_GCC=${ARM_GCC}
      -DARM_NM=${ARM_NM}
      -DARM_READELF=${ARM_READELF}
      -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/..
      -DTEST_DIR=${CMAKE_CURRENT_BINARY_DIR}/shared-sram4-test
      -P ${CMAKE_CURRENT_SOURCE_DIR}/check_shared_sram4.cmake)
  set_tests_properties(shared_sram4_link PROPERTIES TIMEOUT 30)
endif()

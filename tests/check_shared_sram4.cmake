file(MAKE_DIRECTORY "${TEST_DIR}")
file(WRITE "${TEST_DIR}/main.c" "void entry(void) {}\n")
file(WRITE "${TEST_DIR}/main.ld" [=[
MEMORY {
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1M
  SRAM4 (rw) : ORIGIN = 0x38000000, LENGTH = 64K
}
SECTIONS { .text : { *(.text*) } > FLASH }
]=])

foreach(core IN ITEMS m7 m4)
  set(elf "${TEST_DIR}/${core}.elf")
  execute_process(COMMAND "${ARM_GCC}" -mcpu=cortex-${core} -mthumb -nostdlib
    -Wl,-e,entry -T "${TEST_DIR}/main.ld"
    -T "${SOURCE_DIR}/cmake/stm32h745-shared-sram4.ld"
    "${TEST_DIR}/main.c" -o "${elf}"
    RESULT_VARIABLE result ERROR_VARIABLE error)
  if(NOT result EQUAL 0)
    message(FATAL_ERROR "${core} link failed: ${error}")
  endif()
  execute_process(COMMAND "${ARM_NM}" -n "${elf}" OUTPUT_VARIABLE symbols
    RESULT_VARIABLE result)
  if(NOT result EQUAL 0 OR NOT symbols MATCHES "3800ffe0 [A-Za-z] os_atomic_shared_state")
    message(FATAL_ERROR "${core} shared lock is at the wrong address: ${symbols}")
  endif()
  execute_process(COMMAND "${ARM_READELF}" -SW "${elf}" OUTPUT_VARIABLE sections
    RESULT_VARIABLE result)
  if(NOT result EQUAL 0 OR NOT sections MATCHES "\\.titan_rtos_shared +NOBITS +3800ffe0 +[0-9a-f]+ +000020")
    message(FATAL_ERROR "${core} shared section is not a 32-byte NOLOAD reservation: ${sections}")
  endif()
endforeach()

file(WRITE "${TEST_DIR}/collision.ld" [=[
MEMORY {
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1M
  SRAM4 (rw) : ORIGIN = 0x38000000, LENGTH = 64K
}
SECTIONS {
  .text : { *(.text*) } > FLASH
  .collision 0x3800fff0 (NOLOAD) : { . += 16; } > SRAM4
}
]=])
execute_process(COMMAND "${ARM_GCC}" -mcpu=cortex-m7 -mthumb -nostdlib
  -Wl,-e,entry -T "${TEST_DIR}/collision.ld"
  -T "${SOURCE_DIR}/cmake/stm32h745-shared-sram4.ld"
  "${TEST_DIR}/main.c" -o "${TEST_DIR}/collision.elf"
  RESULT_VARIABLE result ERROR_VARIABLE error)
if(result EQUAL 0 OR NOT error MATCHES "overlaps")
  message(FATAL_ERROR "A conflicting SRAM4 allocation was not rejected: ${error}")
endif()

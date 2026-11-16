/* Reserve the final cache line of STM32H745 SRAM4 for Peterson's lock. */
SECTIONS
{
  .titan_rtos_shared 0x3800ffe0 (NOLOAD) :
  {
    os_atomic_shared_state = .;
    . += 32;
  } > SRAM4
}

ASSERT(ORIGIN(SRAM4) == 0x38000000 && LENGTH(SRAM4) >= 0x10000,
       "Titan RTOS requires the full STM32H745 SRAM4 region")

/**
 * This file is part of the Titan Project.
 * Copyright (c) 2025 UW SARP
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * @file os/src/internal/mmio.h
 * @authors Aaron McBride
 * @brief Constants and utilities for interacting with ARMv7-M memory-mapped I/O.
 */

#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "internal/os_assert.h"

/**************************************************************************************************
 * @section MMIO Utilities
 **************************************************************************************************/

/** @brief 32-bit register field type. */
struct field_t {
  uint32_t mask; /** Field bit mask. */
  uint32_t pos;  /** Field position (from LSB). */
};

/**
 * @brief Constructs a 32-bit field struct from a position and width.
 * @param pos (int32_t) The position of the field.
 * @param width (int32_t) The width of the field.
 * @returns (struct field_t) A struct which defines the location of the specified field.
 */
static inline struct field_t make_field(int32_t pos, int32_t width);

/**
 * @brief Writes a value to a field.
 * @param dst (volatile uint32_t*) The location to write to.
 * @param field (struct field_t) The target field.
 * @param value (uint32_t) The value to write to the field.
 */
static inline void write_field(volatile uint32_t* dst, struct field_t field, uint32_t value);

/**
 * @brief Writes a value to a write-only field.
 * @param dst (volatile uint32_t*) The location to write to.
 * @param field (struct field_t) The target field.
 * @param value (uint32_t) The value to write to the field.
 */
static inline void write_wo_field(volatile uint32_t* dst, struct field_t field, uint32_t value);

/**
 * @brief Sets all the bits in a field.
 * @param dst (volatile uint32_t*) The location to write to.
 * @param field (struct field_t) The target field.
 */
static inline void set_field(volatile uint32_t* dst, struct field_t field);

/**
 * @brief Sets all the bits in a write-only field.
 * @param dst (volatile uint32_t*) The location to write to.
 * @param field (struct field_t) The target field.
 */
static inline void set_wo_field(volatile uint32_t* dst, struct field_t field);

/**
 * @brief Clears all the bits in a field.
 * @param dst (volatile uint32_t*) The location to write to.
 * @param field (struct field_t) The target field.
 */
static inline void clear_field(volatile uint32_t* dst, struct field_t field);

/**
 * @brief Toggles all the bits in a field.
 * @param dst (volatile uint32_t*) The location to write to.
 * @param field (struct field_t) The target field.
 */
static inline void toggle_field(volatile uint32_t* dst, struct field_t field);

/**
 * @brief Reads the value of a field
 * @param src (volatile const uint32_t*) The location to read from.
 * @param field (struct field_t) The target field.
 * @returns (uint32_t) The value of @p [field] in @p [src].
 */
static inline uint32_t read_field(volatile const uint32_t* src, struct field_t field);

/**
 * @brief Determines if a field is set.
 * @param src (volatile const uint32_t*) The location to read from.
 * @param field (struct field_t) The target field.
 * @returns (bool) True if all bits  @p [field] in @p [src] are set, or false otherwise.
 */
static inline bool is_field_set(volatile const uint32_t* src, struct field_t field);

/**
 * @brief Determines if a field is clear.
 * @param src (volatile const uint32_t*) The location to read from.
 * @param field (struct field_t) The target field.
 * @returns (bool) True if all bits in @p [field] in @p [src] are clear, or false otherwise.
 */
static inline bool is_field_clr(volatile const uint32_t* src, struct field_t field);

/**************************************************************************************************
 * @section MMIO Constants
 **************************************************************************************************/

/** @subsubsection Nested Vectored Interrupt Controller (NVIC) ************************************/

/** @subsubsection Registers */

extern volatile uint32_t* const NVIC_STIR;           /** @brief Triggers an external interrupt in software. */
extern volatile const uint32_t* const NVIC_ICTR;     /** @brief Reports implemented interrupt register groups. */
extern volatile uint32_t* const NVIC_ISERx[5];       /** @brief Enables external interrupts. */
extern volatile uint32_t* const NVIC_ICERx[5];       /** @brief Disables external interrupts. */
extern volatile uint32_t* const NVIC_ISPRx[5];       /** @brief Sets external interrupts to pending. */
extern volatile uint32_t* const NVIC_ICPRx[5];       /** @brief Clears pending external interrupts. */
extern volatile uint32_t* const NVIC_IPRx[40];       /** @brief Configures external interrupt priorities. */
extern volatile const uint32_t* const NVIC_IABRx[5]; /** @brief Reports active external interrupts. */

/** @subsubsection Fields */

extern const struct field_t NVIC_ICTR_INTLINESNUM; /** @brief Reports the implemented interrupt-group count minus one. */
extern const struct field_t NVIC_STIR_INTID;       /** @brief Selects the external interrupt to pend. */
extern const struct field_t NVIC_IPR_Nx[4];        /** @brief Sets an external interrupt's priority. */

/** @subsubsection Memory Protection Unit (MPU) ***************************************************/

/** @subsubsection Registers */

extern volatile uint32_t* const MPU_MPU_CTRL;        /** @brief Controls MPU operation. */
extern volatile uint32_t* const MPU_MPU_RNR;         /** @brief Selects the MPU region to access. */
extern volatile uint32_t* const MPU_MPU_RBAR;        /** @brief Configures an MPU region's base address. */
extern volatile uint32_t* const MPU_MPU_RASR;        /** @brief Configures an MPU region's size and attributes. */
extern volatile const uint32_t* const MPU_MPU_TYPER; /** @brief Reports the implemented MPU regions. */

/** @subsubsection Fields */

extern const struct field_t MPU_MPU_TYPER_SEPARATE;  /** @brief Indicates separate instruction and data regions. */
extern const struct field_t MPU_MPU_TYPER_DREGION;   /** @brief Reports the number of unified MPU regions. */
extern const struct field_t MPU_MPU_TYPER_IREGION;   /** @brief Reports the number of instruction MPU regions. */
extern const struct field_t MPU_MPU_CTRL_ENABLE;     /** @brief Enables the MPU. */
extern const struct field_t MPU_MPU_CTRL_HFNMIENA;   /** @brief Keeps the MPU enabled during HardFault and NMI. */
extern const struct field_t MPU_MPU_CTRL_PRIVDEFENA; /** @brief Enables the default memory map for privileged access. */
extern const struct field_t MPU_MPU_RNR_REGION;      /** @brief Selects the current MPU region. */
extern const struct field_t MPU_MPU_RBAR_REGION;     /** @brief Selects a region when VALID is set. */
extern const struct field_t MPU_MPU_RBAR_VALID;      /** @brief Enables the RBAR region-number field. */
extern const struct field_t MPU_MPU_RBAR_ADDR;       /** @brief Sets the region base address. */
extern const struct field_t MPU_MPU_RASR_ENABLE;     /** @brief Enables the selected region. */
extern const struct field_t MPU_MPU_RASR_SIZE;       /** @brief Encodes the selected region's size. */
extern const struct field_t MPU_MPU_RASR_SRD;        /** @brief Disables individual region subregions. */
extern const struct field_t MPU_MPU_RASR_B;          /** @brief Sets the memory bufferable attribute. */
extern const struct field_t MPU_MPU_RASR_C;          /** @brief Sets the memory cacheable attribute. */
extern const struct field_t MPU_MPU_RASR_S;          /** @brief Sets the memory shareable attribute. */
extern const struct field_t MPU_MPU_RASR_TEX;        /** @brief Extends the region memory-type encoding. */
extern const struct field_t MPU_MPU_RASR_AP;         /** @brief Sets privileged and unprivileged access permissions. */
extern const struct field_t MPU_MPU_RASR_XN;         /** @brief Prevents instruction fetches from the region. */

/** @subsubsection SysTick Timer (STK) ************************************************************/

/** @subsubsection Registers */

extern volatile uint32_t* const STK_CSR;         /** @brief Controls SysTick and reports its count status. */
extern volatile uint32_t* const STK_RVR;         /** @brief Sets the SysTick counter reload value. */
extern volatile uint32_t* const STK_CVR;         /** @brief Holds the current SysTick counter value. */
extern volatile const uint32_t* const STK_CALIB; /** @brief Reports the SysTick reference calibration. */

/** @subsubsection Fields */

extern const struct field_t STK_CSR_ENABLE;    /** @brief Enables the SysTick counter. */
extern const struct field_t STK_CSR_TICKINT;   /** @brief Enables SysTick exception requests. */
extern const struct field_t STK_CSR_CLKSOURCE; /** @brief Selects the SysTick clock source. */
extern const struct field_t STK_CSR_COUNTFLAG; /** @brief Indicates the counter has reached zero. */
extern const struct field_t STK_RVR_RELOAD;    /** @brief Sets the value reloaded after zero. */
extern const struct field_t STK_CVR_CURRENT;   /** @brief Reports the current counter value. */
extern const struct field_t STK_CALIB_TENMS;   /** @brief Reports the nominal ten-millisecond reload value. */
extern const struct field_t STK_CALIB_SKEW;    /** @brief Indicates TENMS is not exact. */
extern const struct field_t STK_CALIB_NOREF;   /** @brief Indicates no external reference clock is provided. */

/** @subsubsection Floating-Point Unit (FPU) ******************************************************/

/** @subsubsection Registers */

extern volatile uint32_t* const FPU_FPCCR;  /** @brief Controls floating-point context preservation. */
extern volatile uint32_t* const FPU_FPCAR;  /** @brief Holds the lazy floating-point save address. */
extern volatile uint32_t* const FPU_FPDSCR; /** @brief Provides default floating-point control values. */
extern volatile uint32_t* const FPU_CPACR;  /** @brief Controls software access to the FPU coprocessors. */

/** @subsubsection Fields */

extern const struct field_t FPU_FPCCR_LSPACT;  /** @brief Indicates lazy floating-point preservation is active. */
extern const struct field_t FPU_FPCCR_USER;    /** @brief Records whether the interrupted context was unprivileged. */
extern const struct field_t FPU_FPCCR_THREAD;  /** @brief Records whether the interrupted context used Thread mode. */
extern const struct field_t FPU_FPCCR_HFRDY;   /** @brief Allows lazy preservation during HardFault handling. */
extern const struct field_t FPU_FPCCR_MMRDY;   /** @brief Allows lazy preservation during MemManage handling. */
extern const struct field_t FPU_FPCCR_BFRDY;   /** @brief Allows lazy preservation during BusFault handling. */
extern const struct field_t FPU_FPCCR_MONRDY;  /** @brief Allows lazy preservation during DebugMonitor handling. */
extern const struct field_t FPU_FPCCR_LSPEN;   /** @brief Enables lazy floating-point state preservation. */
extern const struct field_t FPU_FPCCR_ASPEN;   /** @brief Enables automatic floating-point state preservation. */
extern const struct field_t FPU_FPCAR_ADDRESS; /** @brief Holds the reserved floating-point stack-frame address. */
extern const struct field_t FPU_FPDSCR_RMODE;  /** @brief Selects the default floating-point rounding mode. */
extern const struct field_t FPU_FPDSCR_FZ;     /** @brief Enables default flush-to-zero mode. */
extern const struct field_t FPU_FPDSCR_DN;     /** @brief Enables default-NaN mode. */
extern const struct field_t FPU_FPDSCR_AHP;    /** @brief Enables default alternative half-precision format. */
extern const struct field_t FPU_CPACR_CP10;    /** @brief Sets coprocessor 10 access permissions. */
extern const struct field_t FPU_CPACR_CP11;    /** @brief Sets coprocessor 11 access permissions. */

/** @subsubsection System Control Block (SCB) *****************************************************/

/** @subsubsection Registers */

extern volatile uint32_t* const SCB_ICSR;         /** @brief Controls and reports exception state. */
extern volatile uint32_t* const SCB_VTOR;         /** @brief Sets the vector table base address. */
extern volatile uint32_t* const SCB_AIRCR;        /** @brief Controls reset, priority grouping, and endianness. */
extern volatile uint32_t* const SCB_SCR;          /** @brief Controls system sleep behavior. */
extern volatile uint32_t* const SCB_CCR;          /** @brief Configures processor traps and core behavior. */
extern volatile uint32_t* const SCB_SHPR1;        /** @brief Sets priorities for system handlers 4 through 6. */
extern volatile uint32_t* const SCB_SHPR2;        /** @brief Sets the SVCall handler priority. */
extern volatile uint32_t* const SCB_SHPR3;        /** @brief Sets PendSV and SysTick handler priorities. */
extern volatile uint32_t* const SCB_SHCSR;        /** @brief Controls and reports system-handler state. */
extern volatile uint32_t* const SCB_CFSR;         /** @brief Reports MemManage, BusFault, and UsageFault status. */
extern volatile uint32_t* const SCB_HFSR;         /** @brief Reports causes of HardFault exceptions. */
extern volatile uint32_t* const SCB_DFSR;         /** @brief Reports debug-event causes. */
extern volatile uint32_t* const SCB_MMFAR;        /** @brief Holds the faulting MemManage address. */
extern volatile uint32_t* const SCB_BFAR;         /** @brief Holds the faulting BusFault address. */
extern volatile uint32_t* const SCB_CSSELR;       /** @brief Selects the cache described by CCSIDR. */
extern volatile uint32_t* const SCB_ACTLR;        /** @brief Controls implementation-defined processor features. */
extern volatile const uint32_t* const SCB_CPUID;  /** @brief Identifies the processor implementation and revision. */
extern volatile const uint32_t* const SCB_CLIDR;  /** @brief Reports cache types implemented at each level. */
extern volatile const uint32_t* const SCB_CTR;    /** @brief Reports cache line and maintenance characteristics. */
extern volatile const uint32_t* const SCB_CCSIDR; /** @brief Reports the selected cache's size and organization. */

/** @subsubsection Fields */

extern const struct field_t SCB_CPUID_REVISION;       /** @brief Reports the processor revision number. */
extern const struct field_t SCB_CPUID_PARTNO;         /** @brief Reports the processor part number. */
extern const struct field_t SCB_CPUID_ARCHITECTURE;   /** @brief Reports the processor architecture identifier. */
extern const struct field_t SCB_CPUID_VARIANT;        /** @brief Reports the processor variant number. */
extern const struct field_t SCB_CPUID_IMPLEMENTER;    /** @brief Reports the implementer code. */
extern const struct field_t SCB_ICSR_VECTACTIVE;      /** @brief Identifies the currently active exception. */
extern const struct field_t SCB_ICSR_RETTOBASE;       /** @brief Indicates no preempted exceptions remain active. */
extern const struct field_t SCB_ICSR_VECTPENDING;     /** @brief Identifies the highest-priority pending exception. */
extern const struct field_t SCB_ICSR_ISRPENDING;      /** @brief Indicates an external interrupt is pending. */
extern const struct field_t SCB_ICSR_ISRPREEMPT;      /** @brief Indicates a pending exception will preempt on return. */
extern const struct field_t SCB_ICSR_PENDSTCLR;       /** @brief Clears the pending SysTick exception. */
extern const struct field_t SCB_ICSR_PENDSTSET;       /** @brief Sets or reports the pending SysTick exception. */
extern const struct field_t SCB_ICSR_PENDSVCLR;       /** @brief Clears the pending PendSV exception. */
extern const struct field_t SCB_ICSR_PENDSVSET;       /** @brief Sets or reports the pending PendSV exception. */
extern const struct field_t SCB_ICSR_NMIPENDSET;      /** @brief Sets or reports the pending NMI exception. */
extern const struct field_t SCB_VTOR_TBLOFF;          /** @brief Sets the vector table base address. */
extern const struct field_t SCB_AIRCR_VECTRESET;      /** @brief Resets processor state without resetting debug logic. */
extern const struct field_t SCB_AIRCR_VECTCLRACTIVE;  /** @brief Clears active exception state for debug use. */
extern const struct field_t SCB_AIRCR_SYSRESETREQ;    /** @brief Requests a system reset. */
extern const struct field_t SCB_AIRCR_PRIGROUP;       /** @brief Selects interrupt priority grouping. */
extern const struct field_t SCB_AIRCR_ENDIANNESS;     /** @brief Reports the configured data endianness. */
extern const struct field_t SCB_AIRCR_VECTKEY;        /** @brief Supplies the key required for AIRCR writes. */
extern const struct field_t SCB_AIRCR_VECTKEYSTAT;    /** @brief Reports the AIRCR key status value. */
extern const struct field_t SCB_SCR_SLEEPONEXIT;      /** @brief Enters sleep when returning to Thread mode. */
extern const struct field_t SCB_SCR_SLEEPDEEP;        /** @brief Selects deep sleep instead of normal sleep. */
extern const struct field_t SCB_SCR_SEVEONPEND;       /** @brief Sends an event when an interrupt becomes pending. */
extern const struct field_t SCB_CCR_NONBASETHRDENA;   /** @brief Allows Thread mode entry with active exceptions. */
extern const struct field_t SCB_CCR_USERSETMPEND;     /** @brief Allows unprivileged software to pend interrupts. */
extern const struct field_t SCB_CCR_UNALIGN_TRP;      /** @brief Traps unaligned word and halfword accesses. */
extern const struct field_t SCB_CCR_DIV_0_TRP;        /** @brief Traps integer division by zero. */
extern const struct field_t SCB_CCR_BFHFNMIGN;        /** @brief Lets priority -1 and -2 handlers ignore data BusFaults. */
extern const struct field_t SCB_CCR_STKALIGN;         /** @brief Enables eight-byte exception stack alignment. */
extern const struct field_t SCB_CCR_DC;               /** @brief Enables the data cache. */
extern const struct field_t SCB_CCR_IC;               /** @brief Enables the instruction cache. */
extern const struct field_t SCB_CCR_BP;               /** @brief Enables branch prediction. */
extern const struct field_t SCB_SHPR2_PRI_11;         /** @brief Sets the SVCall exception priority. */
extern const struct field_t SCB_SHCSR_MEMFAULTACT;    /** @brief Indicates MemManage is active. */
extern const struct field_t SCB_SHCSR_BUSFAULTACT;    /** @brief Indicates BusFault is active. */
extern const struct field_t SCB_SHCSR_USGFAULTACT;    /** @brief Indicates UsageFault is active. */
extern const struct field_t SCB_SHCSR_SVCALLACT;      /** @brief Indicates SVCall is active. */
extern const struct field_t SCB_SHCSR_MONITORACT;     /** @brief Indicates DebugMonitor is active. */
extern const struct field_t SCB_SHCSR_PENDSVACT;      /** @brief Indicates PendSV is active. */
extern const struct field_t SCB_SHCSR_SYSTICKACT;     /** @brief Indicates SysTick is active. */
extern const struct field_t SCB_SHCSR_USGFAULTPENDED; /** @brief Indicates UsageFault is pending. */
extern const struct field_t SCB_SHCSR_MEMFAULTPENDED; /** @brief Indicates MemManage is pending. */
extern const struct field_t SCB_SHCSR_BUSFAULTPENDED; /** @brief Indicates BusFault is pending. */
extern const struct field_t SCB_SHCSR_SVCALLPENDED;   /** @brief Indicates SVCall is pending. */
extern const struct field_t SCB_SHCSR_MEMFAULTENA;    /** @brief Enables the MemManage exception. */
extern const struct field_t SCB_SHCSR_BUSFAULTENA;    /** @brief Enables the BusFault exception. */
extern const struct field_t SCB_SHCSR_USGFAULTENA;    /** @brief Enables the UsageFault exception. */
extern const struct field_t SCB_CFSR_IACCVIOL;        /** @brief Reports an MPU instruction access violation. */
extern const struct field_t SCB_CFSR_DACCVIOL;        /** @brief Reports an MPU data access violation. */
extern const struct field_t SCB_CFSR_MUNSTKERR;       /** @brief Reports an MPU fault during exception unstacking. */
extern const struct field_t SCB_CFSR_MSTKERR;         /** @brief Reports an MPU fault during exception stacking. */
extern const struct field_t SCB_CFSR_MLSPERR;         /** @brief Reports an MPU fault during lazy FP preservation. */
extern const struct field_t SCB_CFSR_MMARVALID;       /** @brief Indicates MMFAR contains a valid fault address. */
extern const struct field_t SCB_CFSR_IBUSERR;         /** @brief Instruction bus error. */
extern const struct field_t SCB_CFSR_PRECISERR;       /** @brief Precise data bus error. */
extern const struct field_t SCB_CFSR_IMPRECISERR;     /** @brief Imprecise data bus error. */
extern const struct field_t SCB_CFSR_UNSTKERR;        /** @brief Bus fault on unstacking for a return from exception. */
extern const struct field_t SCB_CFSR_STKERR;          /** @brief Bus fault on stacking for exception entry. */
extern const struct field_t SCB_CFSR_LSPERR;          /** @brief Bus fault on floating-point lazy state preservation. */
extern const struct field_t SCB_CFSR_BFARVALID;       /** @brief Bus fault address register (BFAR) valid flag. */
extern const struct field_t SCB_CFSR_UNDEFINSTR;      /** @brief Undefined instruction usage fault. */
extern const struct field_t SCB_CFSR_INVSTATE;        /** @brief Invalid state usage fault. */
extern const struct field_t SCB_CFSR_INVPC;           /** @brief Invalid PC load usage fault. */
extern const struct field_t SCB_CFSR_NOCP;            /** @brief No coprocessor usage fault. */
extern const struct field_t SCB_CFSR_UNALIGNED;       /** @brief Unaligned access usage fault. */
extern const struct field_t SCB_CFSR_DIVBYZERO;       /** @brief Divide by zero usage fault. */
extern const struct field_t SCB_HFSR_VECTTBL;         /** @brief Reports a BusFault while reading the vector table. */
extern const struct field_t SCB_HFSR_FORCED;          /** @brief Reports escalation of a configurable fault. */
extern const struct field_t SCB_HFSR_DEBUGEVT;        /** @brief Reports a debug event promoted to HardFault. */
extern const struct field_t SCB_DFSR_HALTED;          /** @brief Reports a halt request or debug instruction. */
extern const struct field_t SCB_DFSR_BKPT;            /** @brief Reports a breakpoint event. */
extern const struct field_t SCB_DFSR_DWTTRAP;         /** @brief Reports a DWT match event. */
extern const struct field_t SCB_DFSR_VCATCH;          /** @brief Reports a vector-catch event. */
extern const struct field_t SCB_DFSR_EXTERNAL;        /** @brief Reports an external debug request. */
extern const struct field_t SCB_ACTLR_DISFOLD;        /** @brief Disables dual-issue execution. */
extern const struct field_t SCB_ACTLR_FPEXCODIS;      /** @brief Disables FPU exception-output signals. */
extern const struct field_t SCB_ACTLR_DISRAMODE;      /** @brief Disables dynamic read allocation. */
extern const struct field_t SCB_ACTLR_DISITMATBFLUSH; /** @brief Disables ITM/DWT flushing on TPIU backpressure. */
extern const struct field_t SCB_CLIDR_LOUIS;          /** @brief Reports the level of unification for inner shareability. */
extern const struct field_t SCB_CLIDR_LOC;            /** @brief Reports the level of coherency. */
extern const struct field_t SCB_CLIDR_LOUU;           /** @brief Reports the level of unification for uniprocessors. */
extern const struct field_t SCB_CTR_IMINLINE;         /** @brief Encodes the minimum instruction-cache line size. */
extern const struct field_t SCB_CTR_DMINLINE;         /** @brief Encodes the minimum data-cache line size. */
extern const struct field_t SCB_CTR_ERG;              /** @brief Encodes the exclusive-reservation granule. */
extern const struct field_t SCB_CTR_CWG;              /** @brief Encodes the cache write-back granule. */
extern const struct field_t SCB_CTR_FORMAT;           /** @brief Identifies the CTR register format. */
extern const struct field_t SCB_CCSIDR_LINESIZE;      /** @brief Encodes the cache line size. */
extern const struct field_t SCB_CCSIDR_ASSOCIATIVITY; /** @brief Reports the cache ways minus one. */
extern const struct field_t SCB_CCSIDR_NUMSETS;       /** @brief Reports the cache sets minus one. */
extern const struct field_t SCB_CCSIDR_WA;            /** @brief Indicates write-allocation support. */
extern const struct field_t SCB_CCSIDR_RA;            /** @brief Indicates read-allocation support. */
extern const struct field_t SCB_CCSIDR_WB;            /** @brief Indicates write-back support. */
extern const struct field_t SCB_CCSIDR_WT;            /** @brief Indicates write-through support. */
extern const struct field_t SCB_CSSELR_IND;           /** @brief Selects the instruction or data cache. */
extern const struct field_t SCB_CSSELR_LEVEL;         /** @brief Selects the cache level. */
extern const struct field_t SCB_SHPR1_PRI_x[7];       /** @brief Sets priorities for system handlers 4 through 6. */
extern const struct field_t SCB_SHPR3_PRI_1x[6];      /** @brief Sets PendSV and SysTick exception priorities. */
extern const struct field_t SCB_CLIDR_CLx[8];         /** @brief Reports the cache type implemented at level x. */

/** @subsubsection Debug (DBG) ********************************************************************/

/** @subsubsection Registers */

extern volatile uint32_t* const DBG_DHCSR; /** @brief Controls halting debug and reports core state. */
extern volatile uint32_t* const DBG_DCRSR; /** @brief Selects a core register for debug transfer. */
extern volatile uint32_t* const DBG_DCRDR; /** @brief Transfers core-register data during debug. */
extern volatile uint32_t* const DBG_DEMCR; /** @brief Controls vector catch, DebugMonitor, and trace. */

/** @subsubsection Fields */

extern const struct field_t DBG_DHCSR_C_DEBUGEN;    /** @brief Enables halting debug. */
extern const struct field_t DBG_DHCSR_C_HALT;       /** @brief Requests the core to halt. */
extern const struct field_t DBG_DHCSR_C_STEP;       /** @brief Enables single-instruction stepping. */
extern const struct field_t DBG_DHCSR_C_MASKINTS;   /** @brief Masks interrupts while stepping. */
extern const struct field_t DBG_DHCSR_C_SNAPSTALL;  /** @brief Allows imprecise entry to Debug state. */
extern const struct field_t DBG_DHCSR_S_REGRDY;     /** @brief Indicates a core-register transfer has completed. */
extern const struct field_t DBG_DHCSR_S_HALT;       /** @brief Indicates the core is halted in Debug state. */
extern const struct field_t DBG_DHCSR_S_SLEEP;      /** @brief Indicates the core is sleeping. */
extern const struct field_t DBG_DHCSR_S_LOCKUP;     /** @brief Indicates the core is locked up. */
extern const struct field_t DBG_DHCSR_S_RETIRE_ST;  /** @brief Indicates an instruction has retired since the last read. */
extern const struct field_t DBG_DHCSR_S_RESET_ST;   /** @brief Indicates a reset occurred since the last read. */
extern const struct field_t DBG_DHCSR_DBGKEY;       /** @brief Supplies the key required for DHCSR writes. */
extern const struct field_t DBG_DCRSR_REGSEL;       /** @brief Selects the core register to transfer. */
extern const struct field_t DBG_DCRSR_REGWnR;       /** @brief Selects a write instead of a read transfer. */
extern const struct field_t DBG_DCRDR_DBGTMP;       /** @brief Holds data for a core-register transfer. */
extern const struct field_t DBG_DEMCR_VC_CORERESET; /** @brief Halts the core on reset. */
extern const struct field_t DBG_DEMCR_VC_MMERR;     /** @brief Halts the core on a MemManage fault. */
extern const struct field_t DBG_DEMCR_VC_NOCPERR;   /** @brief Halts the core on a coprocessor fault. */
extern const struct field_t DBG_DEMCR_VC_CHKERR;    /** @brief Halts the core on a checking fault. */
extern const struct field_t DBG_DEMCR_VC_STATERR;   /** @brief Halts the core on a state fault. */
extern const struct field_t DBG_DEMCR_VC_BUSERR;    /** @brief Halts the core on a BusFault. */
extern const struct field_t DBG_DEMCR_VC_INTERR;    /** @brief Halts the core on an exception-entry fault. */
extern const struct field_t DBG_DEMCR_VC_HARDERR;   /** @brief Halts the core on a HardFault. */
extern const struct field_t DBG_DEMCR_MON_EN;       /** @brief Enables the DebugMonitor exception. */
extern const struct field_t DBG_DEMCR_MON_PEND;     /** @brief Sets or reports a pending DebugMonitor exception. */
extern const struct field_t DBG_DEMCR_MON_STEP;     /** @brief Enables DebugMonitor single stepping. */
extern const struct field_t DBG_DEMCR_MON_REQ;      /** @brief Requests a DebugMonitor exception. */
extern const struct field_t DBG_DEMCR_TRCENA;       /** @brief Enables DWT, ITM, ETM, and TPIU trace blocks. */

/** @subsubsection Auxiliary Control (AC) *********************************************************/

/** @subsubsection Registers */

extern volatile uint32_t* const AC_ITCMCR; /** @brief Controls instruction TCM behavior. */
extern volatile uint32_t* const AC_DTCMCR; /** @brief Controls data TCM behavior. */
extern volatile uint32_t* const AC_AHBPCR; /** @brief Controls the AHB peripheral port. */
extern volatile uint32_t* const AC_CACR;   /** @brief Controls implementation-defined cache behavior. */
extern volatile uint32_t* const AC_AHBSCR; /** @brief Controls AHB slave-port priority. */
extern volatile uint32_t* const AC_ABFSR;  /** @brief Reports faults on processor bus interfaces. */

/** @subsubsection Fields */

extern const struct field_t AC_ITCMCR_EN;        /** @brief Enables instruction TCM. */
extern const struct field_t AC_ITCMCR_RMW;       /** @brief Enables instruction-TCM read-modify-write ECC handling. */
extern const struct field_t AC_ITCMCR_RETEN;     /** @brief Enables instruction-TCM retry on correctable ECC errors. */
extern const struct field_t AC_ITCMCR_SZ;        /** @brief Reports the implemented instruction-TCM size. */
extern const struct field_t AC_DTCMCR_EN;        /** @brief Enables data TCM. */
extern const struct field_t AC_DTCMCR_RMW;       /** @brief Enables data-TCM read-modify-write ECC handling. */
extern const struct field_t AC_DTCMCR_RETEN;     /** @brief Enables data-TCM retry on correctable ECC errors. */
extern const struct field_t AC_DTCMCR_SZ;        /** @brief Reports the implemented data-TCM size. */
extern const struct field_t AC_AHBPCR_EN;        /** @brief Enables the AHB peripheral port. */
extern const struct field_t AC_AHBPCR_SZ;        /** @brief Reports the implemented AHB peripheral-port size. */
extern const struct field_t AC_CACR_SIWT;        /** @brief Enables shared-memory write-through behavior. */
extern const struct field_t AC_CACR_ECCEN;       /** @brief Controls cache ECC, where zero enables checking. */
extern const struct field_t AC_CACR_FORCEWT;     /** @brief Forces write-through behavior for cacheable stores. */
extern const struct field_t AC_AHBSCR_CTL;       /** @brief Selects the AHB slave-port arbitration policy. */
extern const struct field_t AC_AHBSCR_TPRI;      /** @brief Sets the transfer-priority threshold. */
extern const struct field_t AC_AHBSCR_INITCOUNT; /** @brief Sets the fairness counter reload value. */
extern const struct field_t AC_ABFSR_ITCM;       /** @brief Reports an instruction-TCM interface fault. */
extern const struct field_t AC_ABFSR_DTCM;       /** @brief Reports a data-TCM interface fault. */
extern const struct field_t AC_ABFSR_AHBP;       /** @brief Reports an AHB peripheral-port fault. */
extern const struct field_t AC_ABFSR_AXIM;       /** @brief Reports an AXI master-interface fault. */
extern const struct field_t AC_ABFSR_EPPB;       /** @brief Reports an external PPB interface fault. */
extern const struct field_t AC_ABFSR_AXIMTYPE;   /** @brief Identifies the faulting AXI access type. */

/** @subsubsection RAMECC *************************************************************************/

/** @subsubsection Registers */

extern volatile uint32_t* const RAMECCx_IER[4];             /** @brief Enables interrupts for RAMECC controller x. */
extern volatile uint32_t* const RAMECCx_MxCR[4][6];         /** @brief Configures a RAMECC controller monitor. */
extern volatile uint32_t* const RAMECCx_MxSR[4][6];         /** @brief Reports and clears a RAMECC monitor's status. */
extern volatile const uint32_t* const RAMECCx_MxFAR[4][6];  /** @brief Reports a RAMECC monitor's failing address. */
extern volatile const uint32_t* const RAMECCx_MxFDRL[4][6]; /** @brief Reports a RAMECC monitor's failing data low word. */
extern volatile const uint32_t* const RAMECCx_MxFDRH[4][6]; /** @brief Reports a RAMECC monitor's failing data high word. */
extern volatile const uint32_t* const RAMECCx_MxFECR[4][6]; /** @brief Reports a RAMECC monitor's failing ECC code. */

/** @subsubsection Fields */

extern const struct field_t RAMECC_IER_GIE;        /** @brief Enables all controller-wide ECC interrupts. */
extern const struct field_t RAMECC_IER_GECCSEIE;   /** @brief Enables controller-wide single-error interrupts. */
extern const struct field_t RAMECC_IER_GECCDEIE;   /** @brief Enables controller-wide double-error interrupts. */
extern const struct field_t RAMECC_IER_GECCDEBWIE; /** @brief Enables controller-wide byte-write error interrupts. */
extern const struct field_t RAMECC_MCR_ECCSEIE;    /** @brief Enables single-error interrupts for a monitor. */
extern const struct field_t RAMECC_MCR_ECCDEIE;    /** @brief Enables double-error interrupts for a monitor. */
extern const struct field_t RAMECC_MCR_ECCDEBWIE;  /** @brief Enables byte-write error interrupts for a monitor. */
extern const struct field_t RAMECC_MCR_ECCELEN;    /** @brief Enables error capture for a monitor. */
extern const struct field_t RAMECC_MSR_SEDCF;      /** @brief Reports a corrected single-bit ECC error. */
extern const struct field_t RAMECC_MSR_DEDF;       /** @brief Reports a detected double-bit ECC error. */
extern const struct field_t RAMECC_MSR_DEBWDF;     /** @brief Reports a byte-write double-bit ECC error. */
extern const struct field_t RAMECC_MFAR_FADD;      /** @brief Reports the failing address. */
extern const struct field_t RAMECC_MFDRL_FDATAL;   /** @brief Reports the failing data low word. */
extern const struct field_t RAMECC_MFDRH_FDATAH;   /** @brief Reports the failing data high word. */
extern const struct field_t RAMECC_MFECR_FEC;      /** @brief Reports the failing ECC code. */

/** @internal *************************************************************************************/

static inline struct field_t make_field(const int32_t pos, const int32_t width) {
  os_assert((pos >= 0) && (pos < 32));
  os_assert((width > 0) && (width <= 32));
  os_assert((pos + width) <= 32);
  return (struct field_t) {
    .mask = ((UINT64_C(1) << width) - 1U) << pos,
    .pos = pos
  };
}

static inline void write_field(volatile uint32_t* const dst, const struct field_t field, const uint32_t value) {
  os_assert(dst != NULL);
  os_assert((value << field.pos) <= field.mask);
  *dst = (*dst & ~field.mask) | ((value << field.pos) & field.mask);
}

static inline void write_wo_field(volatile uint32_t* const dst, const struct field_t field, const uint32_t value) {
  os_assert(dst != NULL);
  os_assert((value << field.pos) <= field.mask);
  *dst = (value << field.pos) & field.mask;
}

static inline void set_field(volatile uint32_t* const dst, const struct field_t field) {
  os_assert(dst != NULL);
  *dst |= field.mask;
}

static inline void set_wo_field(volatile uint32_t* const dst, const struct field_t field) {
  os_assert(dst != NULL);
  *dst = field.mask;
}

static inline void clear_field(volatile uint32_t* const dst, const struct field_t field) {
  os_assert(dst != NULL);
  *dst &= ~field.mask;
}

static inline void toggle_field(volatile uint32_t* const dst, const struct field_t field) {
  os_assert(dst != NULL);
  *dst ^= field.mask;
}

static inline uint32_t read_field(volatile const uint32_t* const src, const struct field_t field) {
  os_assert(src != NULL);
  return (*src & field.mask) >> field.pos;
}

static inline bool is_field_set(volatile const uint32_t* const src, const struct field_t field) {
  os_assert(src != NULL);
  return (*src & field.mask) == field.mask;
}

static inline bool is_field_clr(volatile const uint32_t* const src, const struct field_t field) {
  os_assert(src != NULL);
  return (*src & field.mask) == 0U;
}

/** @endinternal **********************************************************************************/

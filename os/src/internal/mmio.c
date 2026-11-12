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
 * @file os/src/internal/mmio.c
 * @authors Aaron McBride
 * @brief Definition of MMIO constants (registers/fields).
 */

#include "internal/mmio.h"

/* Nested Vectored Interrupt Controller (NVIC) ****************************************************/

/* Registers */

volatile uint32_t* const NVIC_STIR       = (volatile uint32_t*)0xE000EF00U;
volatile const uint32_t* const NVIC_ICTR = (volatile const uint32_t*)0xE000E004U;

volatile uint32_t* const NVIC_ISERx[5] = {
  [0] = (volatile uint32_t*)0xE000E100U,
  [1] = (volatile uint32_t*)0xE000E104U,
  [2] = (volatile uint32_t*)0xE000E108U,
  [3] = (volatile uint32_t*)0xE000E10CU,
  [4] = (volatile uint32_t*)0xE000E110U,
};

volatile uint32_t* const NVIC_ICERx[5] = {
  [0] = (volatile uint32_t*)0xE000E180U,
  [1] = (volatile uint32_t*)0xE000E184U,
  [2] = (volatile uint32_t*)0xE000E188U,
  [3] = (volatile uint32_t*)0xE000E18CU,
  [4] = (volatile uint32_t*)0xE000E190U,
};

volatile uint32_t* const NVIC_ISPRx[5] = {
  [0] = (volatile uint32_t*)0xE000E200U,
  [1] = (volatile uint32_t*)0xE000E204U,
  [2] = (volatile uint32_t*)0xE000E208U,
  [3] = (volatile uint32_t*)0xE000E20CU,
  [4] = (volatile uint32_t*)0xE000E210U,
};

volatile uint32_t* const NVIC_ICPRx[5] = {
  [0] = (volatile uint32_t*)0xE000E280U,
  [1] = (volatile uint32_t*)0xE000E284U,
  [2] = (volatile uint32_t*)0xE000E288U,
  [3] = (volatile uint32_t*)0xE000E28CU,
  [4] = (volatile uint32_t*)0xE000E290U,
};

volatile uint32_t* const NVIC_IPRx[40] = {
  [0]  = (volatile uint32_t*)0xE000E400U,
  [1]  = (volatile uint32_t*)0xE000E404U,
  [2]  = (volatile uint32_t*)0xE000E408U,
  [3]  = (volatile uint32_t*)0xE000E40CU,
  [4]  = (volatile uint32_t*)0xE000E410U,
  [5]  = (volatile uint32_t*)0xE000E414U,
  [6]  = (volatile uint32_t*)0xE000E418U,
  [7]  = (volatile uint32_t*)0xE000E41CU,
  [8]  = (volatile uint32_t*)0xE000E420U,
  [9]  = (volatile uint32_t*)0xE000E424U,
  [10] = (volatile uint32_t*)0xE000E428U,
  [11] = (volatile uint32_t*)0xE000E42CU,
  [12] = (volatile uint32_t*)0xE000E430U,
  [13] = (volatile uint32_t*)0xE000E434U,
  [14] = (volatile uint32_t*)0xE000E438U,
  [15] = (volatile uint32_t*)0xE000E43CU,
  [16] = (volatile uint32_t*)0xE000E440U,
  [17] = (volatile uint32_t*)0xE000E444U,
  [18] = (volatile uint32_t*)0xE000E448U,
  [19] = (volatile uint32_t*)0xE000E44CU,
  [20] = (volatile uint32_t*)0xE000E450U,
  [21] = (volatile uint32_t*)0xE000E454U,
  [22] = (volatile uint32_t*)0xE000E458U,
  [23] = (volatile uint32_t*)0xE000E45CU,
  [24] = (volatile uint32_t*)0xE000E460U,
  [25] = (volatile uint32_t*)0xE000E464U,
  [26] = (volatile uint32_t*)0xE000E468U,
  [27] = (volatile uint32_t*)0xE000E46CU,
  [28] = (volatile uint32_t*)0xE000E470U,
  [29] = (volatile uint32_t*)0xE000E474U,
  [30] = (volatile uint32_t*)0xE000E478U,
  [31] = (volatile uint32_t*)0xE000E47CU,
  [32] = (volatile uint32_t*)0xE000E480U,
  [33] = (volatile uint32_t*)0xE000E484U,
  [34] = (volatile uint32_t*)0xE000E488U,
  [35] = (volatile uint32_t*)0xE000E48CU,
  [36] = (volatile uint32_t*)0xE000E490U,
  [37] = (volatile uint32_t*)0xE000E494U,
  [38] = (volatile uint32_t*)0xE000E498U,
  [39] = (volatile uint32_t*)0xE000E49CU,
};

volatile const uint32_t* const NVIC_IABRx[5] = {
  [0] = (volatile const uint32_t*)0xE000E300U,
  [1] = (volatile const uint32_t*)0xE000E304U,
  [2] = (volatile const uint32_t*)0xE000E308U,
  [3] = (volatile const uint32_t*)0xE000E30CU,
  [4] = (volatile const uint32_t*)0xE000E310U,
};

/* Fields */

const struct field_t NVIC_ICTR_INTLINESNUM = {.mask = 0x0000000FU, .pos = 0};
const struct field_t NVIC_STIR_INTID       = {.mask = 0x000001FFU, .pos = 0};

const struct field_t NVIC_IPR_Nx[4] = {
  [0] = {.mask = 0x000000FFU, .pos = 0},
  [1] = {.mask = 0x0000FF00U, .pos = 8},
  [2] = {.mask = 0x00FF0000U, .pos = 16},
  [3] = {.mask = 0xFF000000U, .pos = 24},
};

/* Memory Protection Unit (MPU) *******************************************************************/

/* Registers */

volatile uint32_t* const MPU_MPU_CTRL        = (volatile uint32_t*)0xE000ED94U;
volatile uint32_t* const MPU_MPU_RNR         = (volatile uint32_t*)0xE000ED98U;
volatile uint32_t* const MPU_MPU_RBAR        = (volatile uint32_t*)0xE000ED9CU;
volatile uint32_t* const MPU_MPU_RASR        = (volatile uint32_t*)0xE000EDA0U;
volatile const uint32_t* const MPU_MPU_TYPER = (volatile const uint32_t*)0xE000ED90U;

/* Fields */

const struct field_t MPU_MPU_TYPER_SEPARATE  = {.mask = 0x00000001U, .pos = 0};
const struct field_t MPU_MPU_TYPER_DREGION   = {.mask = 0x0000FF00U, .pos = 8};
const struct field_t MPU_MPU_TYPER_IREGION   = {.mask = 0x00FF0000U, .pos = 16};
const struct field_t MPU_MPU_CTRL_ENABLE     = {.mask = 0x00000001U, .pos = 0};
const struct field_t MPU_MPU_CTRL_HFNMIENA   = {.mask = 0x00000002U, .pos = 1};
const struct field_t MPU_MPU_CTRL_PRIVDEFENA = {.mask = 0x00000004U, .pos = 2};
const struct field_t MPU_MPU_RNR_REGION      = {.mask = 0x000000FFU, .pos = 0};
const struct field_t MPU_MPU_RBAR_REGION     = {.mask = 0x0000000FU, .pos = 0};
const struct field_t MPU_MPU_RBAR_VALID      = {.mask = 0x00000010U, .pos = 4};
const struct field_t MPU_MPU_RBAR_ADDR       = {.mask = 0xFFFFFFE0U, .pos = 5};
const struct field_t MPU_MPU_RASR_ENABLE     = {.mask = 0x00000001U, .pos = 0};
const struct field_t MPU_MPU_RASR_SIZE       = {.mask = 0x0000003EU, .pos = 1};
const struct field_t MPU_MPU_RASR_SRD        = {.mask = 0x0000FF00U, .pos = 8};
const struct field_t MPU_MPU_RASR_B          = {.mask = 0x00010000U, .pos = 16};
const struct field_t MPU_MPU_RASR_C          = {.mask = 0x00020000U, .pos = 17};
const struct field_t MPU_MPU_RASR_S          = {.mask = 0x00040000U, .pos = 18};
const struct field_t MPU_MPU_RASR_TEX        = {.mask = 0x00380000U, .pos = 19};
const struct field_t MPU_MPU_RASR_AP         = {.mask = 0x07000000U, .pos = 24};
const struct field_t MPU_MPU_RASR_XN         = {.mask = 0x10000000U, .pos = 28};

/* Systick Timer (STK) ****************************************************************************/

/* Registers */

volatile uint32_t* const STK_CSR         = (volatile uint32_t*)0xE000E010U;
volatile uint32_t* const STK_RVR         = (volatile uint32_t*)0xE000E014U;
volatile uint32_t* const STK_CVR         = (volatile uint32_t*)0xE000E018U;
volatile const uint32_t* const STK_CALIB = (volatile const uint32_t*)0xE000E01CU;

/* Fields */

const struct field_t STK_CSR_ENABLE    = {.mask = 0x00000001U, .pos = 0};
const struct field_t STK_CSR_TICKINT   = {.mask = 0x00000002U, .pos = 1};
const struct field_t STK_CSR_CLKSOURCE = {.mask = 0x00000004U, .pos = 2};
const struct field_t STK_CSR_COUNTFLAG = {.mask = 0x00010000U, .pos = 16};
const struct field_t STK_RVR_RELOAD    = {.mask = 0x00FFFFFFU, .pos = 0};
const struct field_t STK_CVR_CURRENT   = {.mask = 0x00FFFFFFU, .pos = 0};
const struct field_t STK_CALIB_TENMS   = {.mask = 0x00FFFFFFU, .pos = 0};
const struct field_t STK_CALIB_SKEW    = {.mask = 0x40000000U, .pos = 30};
const struct field_t STK_CALIB_NOREF   = {.mask = 0x80000000U, .pos = 31};

/* Floating-Point Unit (FPU) **********************************************************************/

/* Registers */

volatile uint32_t* const FPU_FPCCR  = (volatile uint32_t*)0xE000EF34U;
volatile uint32_t* const FPU_FPCAR  = (volatile uint32_t*)0xE000EF38U;
volatile uint32_t* const FPU_FPDSCR = (volatile uint32_t*)0xE000EF3CU;
volatile uint32_t* const FPU_CPACR  = (volatile uint32_t*)0xE000ED88U;

/* Fields */

const struct field_t FPU_FPCCR_LSPACT  = {.mask = 0x00000001U, .pos = 0};
const struct field_t FPU_FPCCR_USER    = {.mask = 0x00000002U, .pos = 1};
const struct field_t FPU_FPCCR_THREAD  = {.mask = 0x00000008U, .pos = 3};
const struct field_t FPU_FPCCR_HFRDY   = {.mask = 0x00000010U, .pos = 4};
const struct field_t FPU_FPCCR_MMRDY   = {.mask = 0x00000020U, .pos = 5};
const struct field_t FPU_FPCCR_BFRDY   = {.mask = 0x00000040U, .pos = 6};
const struct field_t FPU_FPCCR_MONRDY  = {.mask = 0x00000100U, .pos = 8};
const struct field_t FPU_FPCCR_LSPEN   = {.mask = 0x40000000U, .pos = 30};
const struct field_t FPU_FPCCR_ASPEN   = {.mask = 0x80000000U, .pos = 31};
const struct field_t FPU_FPCAR_ADDRESS = {.mask = 0xFFFFFFF8U, .pos = 3};
const struct field_t FPU_FPDSCR_RMODE  = {.mask = 0x00C00000U, .pos = 22};
const struct field_t FPU_FPDSCR_FZ     = {.mask = 0x01000000U, .pos = 24};
const struct field_t FPU_FPDSCR_DN     = {.mask = 0x02000000U, .pos = 25};
const struct field_t FPU_FPDSCR_AHP    = {.mask = 0x04000000U, .pos = 26};
const struct field_t FPU_CPACR_CP10    = {.mask = 0x00300000U, .pos = 20};
const struct field_t FPU_CPACR_CP11    = {.mask = 0x00C00000U, .pos = 22};

/* System Control Block (SCB) *********************************************************************/

/* Registers */

volatile uint32_t* const SCB_ICSR         = (volatile uint32_t*)0xE000ED04U;
volatile uint32_t* const SCB_VTOR         = (volatile uint32_t*)0xE000ED08U;
volatile uint32_t* const SCB_AIRCR        = (volatile uint32_t*)0xE000ED0CU;
volatile uint32_t* const SCB_SCR          = (volatile uint32_t*)0xE000ED10U;
volatile uint32_t* const SCB_CCR          = (volatile uint32_t*)0xE000ED14U;
volatile uint32_t* const SCB_SHPR1        = (volatile uint32_t*)0xE000ED18U;
volatile uint32_t* const SCB_SHPR2        = (volatile uint32_t*)0xE000ED1CU;
volatile uint32_t* const SCB_SHPR3        = (volatile uint32_t*)0xE000ED20U;
volatile uint32_t* const SCB_SHCSR        = (volatile uint32_t*)0xE000ED24U;
volatile uint32_t* const SCB_CFSR         = (volatile uint32_t*)0xE000ED28U;
volatile uint32_t* const SCB_HFSR         = (volatile uint32_t*)0xE000ED2CU;
volatile uint32_t* const SCB_DFSR         = (volatile uint32_t*)0xE000ED30U;
volatile uint32_t* const SCB_MMFAR        = (volatile uint32_t*)0xE000ED34U;
volatile uint32_t* const SCB_BFAR         = (volatile uint32_t*)0xE000ED38U;
volatile uint32_t* const SCB_CSSELR       = (volatile uint32_t*)0xE000ED84U;
volatile uint32_t* const SCB_ACTLR        = (volatile uint32_t*)0xE000E008U;
volatile const uint32_t* const SCB_CPUID  = (volatile const uint32_t*)0xE000ED00U;
volatile const uint32_t* const SCB_CLIDR  = (volatile const uint32_t*)0xE000ED78U;
volatile const uint32_t* const SCB_CTR    = (volatile const uint32_t*)0xE000ED7CU;
volatile const uint32_t* const SCB_CCSIDR = (volatile const uint32_t*)0xE000ED80U;

/* Fields */

const struct field_t SCB_CPUID_REVISION             = {.mask = 0x0000000FU, .pos = 0};
const struct field_t SCB_CPUID_PARTNO               = {.mask = 0x0000FFF0U, .pos = 4};
const struct field_t SCB_CPUID_ARCHITECTURE         = {.mask = 0x000F0000U, .pos = 16};
const struct field_t SCB_CPUID_VARIANT              = {.mask = 0x00F00000U, .pos = 20};
const struct field_t SCB_CPUID_IMPLEMENTER          = {.mask = 0xFF000000U, .pos = 24};
const struct field_t SCB_ICSR_VECTACTIVE            = {.mask = 0x000001FFU, .pos = 0};
const struct field_t SCB_ICSR_RETTOBASE             = {.mask = 0x00000800U, .pos = 11};
const struct field_t SCB_ICSR_VECTPENDING           = {.mask = 0x001FF000U, .pos = 12};
const struct field_t SCB_ICSR_ISRPENDING            = {.mask = 0x00400000U, .pos = 22};
const struct field_t SCB_ICSR_ISRPREEMPT            = {.mask = 0x00800000U, .pos = 23};
const struct field_t SCB_ICSR_PENDSTCLR             = {.mask = 0x02000000U, .pos = 25};
const struct field_t SCB_ICSR_PENDSTSET             = {.mask = 0x04000000U, .pos = 26};
const struct field_t SCB_ICSR_PENDSVCLR             = {.mask = 0x08000000U, .pos = 27};
const struct field_t SCB_ICSR_PENDSVSET             = {.mask = 0x10000000U, .pos = 28};
const struct field_t SCB_ICSR_NMIPENDSET            = {.mask = 0x80000000U, .pos = 31};
const struct field_t SCB_VTOR_TBLOFF                = {.mask = 0xFFFFFF80U, .pos = 7};
const struct field_t SCB_AIRCR_VECTRESET            = {.mask = 0x00000001U, .pos = 0};
const struct field_t SCB_AIRCR_VECTCLRACTIVE        = {.mask = 0x00000002U, .pos = 1};
const struct field_t SCB_AIRCR_SYSRESETREQ          = {.mask = 0x00000004U, .pos = 2};
const struct field_t SCB_AIRCR_PRIGROUP             = {.mask = 0x00000700U, .pos = 8};
const struct field_t SCB_AIRCR_ENDIANNESS           = {.mask = 0x00008000U, .pos = 15};
const struct field_t SCB_AIRCR_VECTKEY              = {.mask = 0xFFFF0000U, .pos = 16};
const struct field_t SCB_AIRCR_VECTKEYSTAT          = {.mask = 0xFFFF0000U, .pos = 16};
const struct field_t SCB_SCR_SLEEPONEXIT            = {.mask = 0x00000002U, .pos = 1};
const struct field_t SCB_SCR_SLEEPDEEP              = {.mask = 0x00000004U, .pos = 2};
const struct field_t SCB_SCR_SEVEONPEND             = {.mask = 0x00000010U, .pos = 4};
const struct field_t SCB_CCR_NONBASETHRDENA         = {.mask = 0x00000001U, .pos = 0};
const struct field_t SCB_CCR_USERSETMPEND           = {.mask = 0x00000002U, .pos = 1};
const struct field_t SCB_CCR_UNALIGN_TRP            = {.mask = 0x00000008U, .pos = 3};
const struct field_t SCB_CCR_DIV_0_TRP              = {.mask = 0x00000010U, .pos = 4};
const struct field_t SCB_CCR_BFHFNMIGN              = {.mask = 0x00000100U, .pos = 8};
const struct field_t SCB_CCR_STKALIGN               = {.mask = 0x00000200U, .pos = 9};
const struct field_t SCB_CCR_DC                     = {.mask = 0x00010000U, .pos = 16};
const struct field_t SCB_CCR_IC                     = {.mask = 0x00020000U, .pos = 17};
const struct field_t SCB_CCR_BP                     = {.mask = 0x00040000U, .pos = 18};
const struct field_t SCB_SHPR2_PRI_11               = {.mask = 0xFF000000U, .pos = 24};
const struct field_t SCB_SHCSR_MEMFAULTACT          = {.mask = 0x00000001U, .pos = 0};
const struct field_t SCB_SHCSR_BUSFAULTACT          = {.mask = 0x00000002U, .pos = 1};
const struct field_t SCB_SHCSR_USGFAULTACT          = {.mask = 0x00000008U, .pos = 3};
const struct field_t SCB_SHCSR_SVCALLACT            = {.mask = 0x00000080U, .pos = 7};
const struct field_t SCB_SHCSR_MONITORACT           = {.mask = 0x00000100U, .pos = 8};
const struct field_t SCB_SHCSR_PENDSVACT            = {.mask = 0x00000400U, .pos = 10};
const struct field_t SCB_SHCSR_SYSTICKACT           = {.mask = 0x00000800U, .pos = 11};
const struct field_t SCB_SHCSR_USGFAULTPENDED       = {.mask = 0x00001000U, .pos = 12};
const struct field_t SCB_SHCSR_MEMFAULTPENDED       = {.mask = 0x00002000U, .pos = 13};
const struct field_t SCB_SHCSR_BUSFAULTPENDED       = {.mask = 0x00004000U, .pos = 14};
const struct field_t SCB_SHCSR_SVCALLPENDED         = {.mask = 0x00008000U, .pos = 15};
const struct field_t SCB_SHCSR_MEMFAULTENA          = {.mask = 0x00010000U, .pos = 16};
const struct field_t SCB_SHCSR_BUSFAULTENA          = {.mask = 0x00020000U, .pos = 17};
const struct field_t SCB_SHCSR_USGFAULTENA          = {.mask = 0x00040000U, .pos = 18};
const struct field_t SCB_CFSR_IACCVIOL              = {.mask = 0x00000001U, .pos = 0};
const struct field_t SCB_CFSR_DACCVIOL              = {.mask = 0x00000002U, .pos = 1};
const struct field_t SCB_CFSR_MUNSTKERR             = {.mask = 0x00000008U, .pos = 3};
const struct field_t SCB_CFSR_MSTKERR               = {.mask = 0x00000010U, .pos = 4};
const struct field_t SCB_CFSR_MLSPERR               = {.mask = 0x00000020U, .pos = 5};
const struct field_t SCB_CFSR_MMARVALID             = {.mask = 0x00000080U, .pos = 7};
const struct field_t SCB_CFSR_IBUSERR               = {.mask = 0x00000100U, .pos = 8};
const struct field_t SCB_CFSR_PRECISERR             = {.mask = 0x00000200U, .pos = 9};
const struct field_t SCB_CFSR_IMPRECISERR           = {.mask = 0x00000400U, .pos = 10};
const struct field_t SCB_CFSR_UNSTKERR              = {.mask = 0x00000800U, .pos = 11};
const struct field_t SCB_CFSR_STKERR                = {.mask = 0x00001000U, .pos = 12};
const struct field_t SCB_CFSR_LSPERR                = {.mask = 0x00002000U, .pos = 13};
const struct field_t SCB_CFSR_BFARVALID             = {.mask = 0x00008000U, .pos = 15};
const struct field_t SCB_CFSR_UNDEFINSTR            = {.mask = 0x00010000U, .pos = 16};
const struct field_t SCB_CFSR_INVSTATE              = {.mask = 0x00020000U, .pos = 17};
const struct field_t SCB_CFSR_INVPC                 = {.mask = 0x00040000U, .pos = 18};
const struct field_t SCB_CFSR_NOCP                  = {.mask = 0x00080000U, .pos = 19};
const struct field_t SCB_CFSR_UNALIGNED             = {.mask = 0x01000000U, .pos = 24};
const struct field_t SCB_CFSR_DIVBYZERO             = {.mask = 0x02000000U, .pos = 25};
const struct field_t SCB_HFSR_VECTTBL               = {.mask = 0x00000002U, .pos = 1};
const struct field_t SCB_HFSR_FORCED                = {.mask = 0x40000000U, .pos = 30};
const struct field_t SCB_HFSR_DEBUGEVT              = {.mask = 0x80000000U, .pos = 31};
const struct field_t SCB_DFSR_HALTED                = {.mask = 0x00000001U, .pos = 0};
const struct field_t SCB_DFSR_BKPT                  = {.mask = 0x00000002U, .pos = 1};
const struct field_t SCB_DFSR_DWTTRAP               = {.mask = 0x00000004U, .pos = 2};
const struct field_t SCB_DFSR_VCATCH                = {.mask = 0x00000008U, .pos = 3};
const struct field_t SCB_DFSR_EXTERNAL              = {.mask = 0x00000010U, .pos = 4};
const struct field_t SCB_ACTLR_DISFOLD              = {.mask = 0x00000004U, .pos = 2};
const struct field_t SCB_ACTLR_FPEXCODIS            = {.mask = 0x00000400U, .pos = 10};
const struct field_t SCB_ACTLR_DISRAMODE            = {.mask = 0x00000800U, .pos = 11};
const struct field_t SCB_ACTLR_DISITMATBFLUSH       = {.mask = 0x00001000U, .pos = 12};
const struct field_t SCB_CLIDR_LOUIS                = {.mask = 0x00E00000U, .pos = 21};
const struct field_t SCB_CLIDR_LOC                  = {.mask = 0x07000000U, .pos = 24};
const struct field_t SCB_CLIDR_LOUU                 = {.mask = 0x38000000U, .pos = 27};
const struct field_t SCB_CTR_IMINLINE               = {.mask = 0x0000000FU, .pos = 0};
const struct field_t SCB_CTR_DMINLINE               = {.mask = 0x000F0000U, .pos = 16};
const struct field_t SCB_CTR_ERG                    = {.mask = 0x00F00000U, .pos = 20};
const struct field_t SCB_CTR_CWG                    = {.mask = 0x0F000000U, .pos = 24};
const struct field_t SCB_CTR_FORMAT                 = {.mask = 0xE0000000U, .pos = 29};
const struct field_t SCB_CCSIDR_LINESIZE            = {.mask = 0x00000007U, .pos = 0};
const struct field_t SCB_CCSIDR_ASSOCIATIVITY       = {.mask = 0x00001FF8U, .pos = 3};
const struct field_t SCB_CCSIDR_NUMSETS             = {.mask = 0x0FFFE000U, .pos = 13};
const struct field_t SCB_CCSIDR_WA                  = {.mask = 0x10000000U, .pos = 28};
const struct field_t SCB_CCSIDR_RA                  = {.mask = 0x20000000U, .pos = 29};
const struct field_t SCB_CCSIDR_WB                  = {.mask = 0x40000000U, .pos = 30};
const struct field_t SCB_CCSIDR_WT                  = {.mask = 0x80000000U, .pos = 31};
const struct field_t SCB_CSSELR_IND                 = {.mask = 0x00000001U, .pos = 0};
const struct field_t SCB_CSSELR_LEVEL               = {.mask = 0x0000000EU, .pos = 1};

const struct field_t SCB_SHPR1_PRI_x[7] = {
  [4] = {.mask = 0x000000FFU, .pos = 0},
  [5] = {.mask = 0x0000FF00U, .pos = 8},
  [6] = {.mask = 0x00FF0000U, .pos = 16},
};

const struct field_t SCB_SHPR3_PRI_1x[6] = {
  [4] = {.mask = 0x00FF0000U, .pos = 16},
  [5] = {.mask = 0xFF000000U, .pos = 24},
};

const struct field_t SCB_CLIDR_CLx[8] = {
  [1] = {.mask = 0x00000007U, .pos = 0},
  [2] = {.mask = 0x00000038U, .pos = 3},
  [3] = {.mask = 0x000001C0U, .pos = 6},
  [4] = {.mask = 0x00000E00U, .pos = 9},
  [5] = {.mask = 0x00007000U, .pos = 12},
  [6] = {.mask = 0x00038000U, .pos = 15},
  [7] = {.mask = 0x001C0000U, .pos = 18},
};

/* Debug (DBG) ************************************************************************************/

/* Registers */

volatile uint32_t* const DBG_DHCSR = (volatile uint32_t*)0xE000EDF0U;
volatile uint32_t* const DBG_DCRSR = (volatile uint32_t*)0xE000EDF4U;
volatile uint32_t* const DBG_DCRDR = (volatile uint32_t*)0xE000EDF8U;
volatile uint32_t* const DBG_DEMCR = (volatile uint32_t*)0xE000EDFCU;

/* Fields */

const struct field_t DBG_DHCSR_C_DEBUGEN    = {.mask = 0x00000001U, .pos = 0};
const struct field_t DBG_DHCSR_C_HALT       = {.mask = 0x00000002U, .pos = 1};
const struct field_t DBG_DHCSR_C_STEP       = {.mask = 0x00000004U, .pos = 2};
const struct field_t DBG_DHCSR_C_MASKINTS   = {.mask = 0x00000008U, .pos = 3};
const struct field_t DBG_DHCSR_C_SNAPSTALL  = {.mask = 0x00000020U, .pos = 5};
const struct field_t DBG_DHCSR_S_REGRDY     = {.mask = 0x00010000U, .pos = 16};
const struct field_t DBG_DHCSR_S_HALT       = {.mask = 0x00020000U, .pos = 17};
const struct field_t DBG_DHCSR_S_SLEEP      = {.mask = 0x00040000U, .pos = 18};
const struct field_t DBG_DHCSR_S_LOCKUP     = {.mask = 0x00080000U, .pos = 19};
const struct field_t DBG_DHCSR_S_RETIRE_ST  = {.mask = 0x01000000U, .pos = 24};
const struct field_t DBG_DHCSR_S_RESET_ST   = {.mask = 0x02000000U, .pos = 25};
const struct field_t DBG_DHCSR_DBGKEY       = {.mask = 0xFFFF0000U, .pos = 16};
const struct field_t DBG_DCRSR_REGSEL       = {.mask = 0x0000007FU, .pos = 0};
const struct field_t DBG_DCRSR_REGWnR       = {.mask = 0x00010000U, .pos = 16};
const struct field_t DBG_DCRDR_DBGTMP       = {.mask = 0xFFFFFFFFU, .pos = 0};
const struct field_t DBG_DEMCR_VC_CORERESET = {.mask = 0x00000001U, .pos = 0};
const struct field_t DBG_DEMCR_VC_MMERR     = {.mask = 0x00000010U, .pos = 4};
const struct field_t DBG_DEMCR_VC_NOCPERR   = {.mask = 0x00000020U, .pos = 5};
const struct field_t DBG_DEMCR_VC_CHKERR    = {.mask = 0x00000040U, .pos = 6};
const struct field_t DBG_DEMCR_VC_STATERR   = {.mask = 0x00000080U, .pos = 7};
const struct field_t DBG_DEMCR_VC_BUSERR    = {.mask = 0x00000100U, .pos = 8};
const struct field_t DBG_DEMCR_VC_INTERR    = {.mask = 0x00000200U, .pos = 9};
const struct field_t DBG_DEMCR_VC_HARDERR   = {.mask = 0x00000400U, .pos = 10};
const struct field_t DBG_DEMCR_MON_EN       = {.mask = 0x00010000U, .pos = 16};
const struct field_t DBG_DEMCR_MON_PEND     = {.mask = 0x00020000U, .pos = 17};
const struct field_t DBG_DEMCR_MON_STEP     = {.mask = 0x00040000U, .pos = 18};
const struct field_t DBG_DEMCR_MON_REQ      = {.mask = 0x00080000U, .pos = 19};
const struct field_t DBG_DEMCR_TRCENA       = {.mask = 0x01000000U, .pos = 24};

/* Auxiliary Control (AC) *************************************************************************/

/* Registers */

volatile uint32_t* const AC_ITCMCR = (volatile uint32_t*)0xE000EF90U;
volatile uint32_t* const AC_DTCMCR = (volatile uint32_t*)0xE000EF94U;
volatile uint32_t* const AC_AHBPCR = (volatile uint32_t*)0xE000EF98U;
volatile uint32_t* const AC_CACR   = (volatile uint32_t*)0xE000EF9CU;
volatile uint32_t* const AC_AHBSCR = (volatile uint32_t*)0xE000EFA0U;
volatile uint32_t* const AC_ABFSR  = (volatile uint32_t*)0xE000EFA8U;

/* Fields */

const struct field_t AC_ITCMCR_EN        = {.mask = 0x00000001U, .pos = 0};
const struct field_t AC_ITCMCR_RMW       = {.mask = 0x00000002U, .pos = 1};
const struct field_t AC_ITCMCR_RETEN     = {.mask = 0x00000004U, .pos = 2};
const struct field_t AC_ITCMCR_SZ        = {.mask = 0x00000078U, .pos = 3};
const struct field_t AC_DTCMCR_EN        = {.mask = 0x00000001U, .pos = 0};
const struct field_t AC_DTCMCR_RMW       = {.mask = 0x00000002U, .pos = 1};
const struct field_t AC_DTCMCR_RETEN     = {.mask = 0x00000004U, .pos = 2};
const struct field_t AC_DTCMCR_SZ        = {.mask = 0x00000078U, .pos = 3};
const struct field_t AC_AHBPCR_EN        = {.mask = 0x00000001U, .pos = 0};
const struct field_t AC_AHBPCR_SZ        = {.mask = 0x0000000EU, .pos = 1};
const struct field_t AC_CACR_SIWT        = {.mask = 0x00000001U, .pos = 0};
const struct field_t AC_CACR_ECCEN       = {.mask = 0x00000002U, .pos = 1};
const struct field_t AC_CACR_FORCEWT     = {.mask = 0x00000004U, .pos = 2};
const struct field_t AC_AHBSCR_CTL       = {.mask = 0x00000003U, .pos = 0};
const struct field_t AC_AHBSCR_TPRI      = {.mask = 0x000007FCU, .pos = 2};
const struct field_t AC_AHBSCR_INITCOUNT = {.mask = 0x0000F800U, .pos = 11};
const struct field_t AC_ABFSR_ITCM       = {.mask = 0x00000001U, .pos = 0};
const struct field_t AC_ABFSR_DTCM       = {.mask = 0x00000002U, .pos = 1};
const struct field_t AC_ABFSR_AHBP       = {.mask = 0x00000004U, .pos = 2};
const struct field_t AC_ABFSR_AXIM       = {.mask = 0x00000008U, .pos = 3};
const struct field_t AC_ABFSR_EPPB       = {.mask = 0x00000010U, .pos = 4};
const struct field_t AC_ABFSR_AXIMTYPE   = {.mask = 0x00000300U, .pos = 8};

/* RAMECC *****************************************************************************************/

/* Registers */

volatile uint32_t* const RAMECCx_IER[4] = {
  [1] = (volatile uint32_t*)0x52009000U,
  [2] = (volatile uint32_t*)0x48023000U,
  [3] = (volatile uint32_t*)0x58027000U,
};

volatile uint32_t* const RAMECCx_MxCR[4][6] = {
  [1][1] = (volatile uint32_t*)0x52009020U,
  [1][2] = (volatile uint32_t*)0x52009040U,
  [1][3] = (volatile uint32_t*)0x52009060U,
  [1][4] = (volatile uint32_t*)0x52009080U,
  [1][5] = (volatile uint32_t*)0x520090A0U,
  [2][1] = (volatile uint32_t*)0x48023020U,
  [2][2] = (volatile uint32_t*)0x48023040U,
  [2][3] = (volatile uint32_t*)0x48023060U,
  [2][4] = (volatile uint32_t*)0x48023080U,
  [2][5] = (volatile uint32_t*)0x480230A0U,
  [3][1] = (volatile uint32_t*)0x58027020U,
  [3][2] = (volatile uint32_t*)0x58027040U,
};

volatile uint32_t* const RAMECCx_MxSR[4][6] = {
  [1][1] = (volatile uint32_t*)0x52009024U,
  [1][2] = (volatile uint32_t*)0x52009044U,
  [1][3] = (volatile uint32_t*)0x52009064U,
  [1][4] = (volatile uint32_t*)0x52009084U,
  [1][5] = (volatile uint32_t*)0x520090A4U,
  [2][1] = (volatile uint32_t*)0x48023024U,
  [2][2] = (volatile uint32_t*)0x48023044U,
  [2][3] = (volatile uint32_t*)0x48023064U,
  [2][4] = (volatile uint32_t*)0x48023084U,
  [2][5] = (volatile uint32_t*)0x480230A4U,
  [3][1] = (volatile uint32_t*)0x58027024U,
  [3][2] = (volatile uint32_t*)0x58027044U,
};

volatile const uint32_t* const RAMECCx_MxFAR[4][6] = {
  [1][1] = (volatile const uint32_t*)0x52009028U,
  [1][2] = (volatile const uint32_t*)0x52009048U,
  [1][3] = (volatile const uint32_t*)0x52009068U,
  [1][4] = (volatile const uint32_t*)0x52009088U,
  [1][5] = (volatile const uint32_t*)0x520090A8U,
  [2][1] = (volatile const uint32_t*)0x48023028U,
  [2][2] = (volatile const uint32_t*)0x48023048U,
  [2][3] = (volatile const uint32_t*)0x48023068U,
  [2][4] = (volatile const uint32_t*)0x48023088U,
  [2][5] = (volatile const uint32_t*)0x480230A8U,
  [3][1] = (volatile const uint32_t*)0x58027028U,
  [3][2] = (volatile const uint32_t*)0x58027048U,
};

volatile const uint32_t* const RAMECCx_MxFDRL[4][6] = {
  [1][1] = (volatile const uint32_t*)0x5200902CU,
  [1][2] = (volatile const uint32_t*)0x5200904CU,
  [1][3] = (volatile const uint32_t*)0x5200906CU,
  [1][4] = (volatile const uint32_t*)0x5200908CU,
  [1][5] = (volatile const uint32_t*)0x520090ACU,
  [2][1] = (volatile const uint32_t*)0x4802302CU,
  [2][2] = (volatile const uint32_t*)0x4802304CU,
  [2][3] = (volatile const uint32_t*)0x4802306CU,
  [2][4] = (volatile const uint32_t*)0x4802308CU,
  [2][5] = (volatile const uint32_t*)0x480230ACU,
  [3][1] = (volatile const uint32_t*)0x5802702CU,
  [3][2] = (volatile const uint32_t*)0x5802704CU,
};

volatile const uint32_t* const RAMECCx_MxFDRH[4][6] = {
  [1][1] = (volatile const uint32_t*)0x52009030U,
  [1][2] = (volatile const uint32_t*)0x52009050U,
  [1][3] = (volatile const uint32_t*)0x52009070U,
  [1][4] = (volatile const uint32_t*)0x52009090U,
  [1][5] = (volatile const uint32_t*)0x520090B0U,
  [2][1] = (volatile const uint32_t*)0x48023030U,
  [2][2] = (volatile const uint32_t*)0x48023050U,
  [2][3] = (volatile const uint32_t*)0x48023070U,
  [2][4] = (volatile const uint32_t*)0x48023090U,
  [2][5] = (volatile const uint32_t*)0x480230B0U,
  [3][1] = (volatile const uint32_t*)0x58027030U,
  [3][2] = (volatile const uint32_t*)0x58027050U,
};

volatile const uint32_t* const RAMECCx_MxFECR[4][6] = {
  [1][1] = (volatile const uint32_t*)0x52009034U,
  [1][2] = (volatile const uint32_t*)0x52009054U,
  [1][3] = (volatile const uint32_t*)0x52009074U,
  [1][4] = (volatile const uint32_t*)0x52009094U,
  [1][5] = (volatile const uint32_t*)0x520090B4U,
  [2][1] = (volatile const uint32_t*)0x48023034U,
  [2][2] = (volatile const uint32_t*)0x48023054U,
  [2][3] = (volatile const uint32_t*)0x48023074U,
  [2][4] = (volatile const uint32_t*)0x48023094U,
  [2][5] = (volatile const uint32_t*)0x480230B4U,
  [3][1] = (volatile const uint32_t*)0x58027034U,
  [3][2] = (volatile const uint32_t*)0x58027054U,
};

/* Fields */

const struct field_t RAMECC_IER_GIE        = {.mask = 0x00000001U, .pos = 0};
const struct field_t RAMECC_IER_GECCSEIE   = {.mask = 0x00000002U, .pos = 1};
const struct field_t RAMECC_IER_GECCDEIE   = {.mask = 0x00000004U, .pos = 2};
const struct field_t RAMECC_IER_GECCDEBWIE = {.mask = 0x00000008U, .pos = 3};
const struct field_t RAMECC_MCR_ECCSEIE    = {.mask = 0x00000004U, .pos = 2};
const struct field_t RAMECC_MCR_ECCDEIE    = {.mask = 0x00000008U, .pos = 3};
const struct field_t RAMECC_MCR_ECCDEBWIE  = {.mask = 0x00000010U, .pos = 4};
const struct field_t RAMECC_MCR_ECCELEN    = {.mask = 0x00000020U, .pos = 5};
const struct field_t RAMECC_MSR_SEDCF      = {.mask = 0x00000001U, .pos = 0};
const struct field_t RAMECC_MSR_DEDF       = {.mask = 0x00000002U, .pos = 1};
const struct field_t RAMECC_MSR_DEBWDF     = {.mask = 0x00000004U, .pos = 2};
const struct field_t RAMECC_MFAR_FADD      = {.mask = 0xFFFFFFFFU, .pos = 0};
const struct field_t RAMECC_MFDRL_FDATAL   = {.mask = 0xFFFFFFFFU, .pos = 0};
const struct field_t RAMECC_MFDRH_FDATAH   = {.mask = 0xFFFFFFFFU, .pos = 0};
const struct field_t RAMECC_MFECR_FEC      = {.mask = 0xFFFFFFFFU, .pos = 0};
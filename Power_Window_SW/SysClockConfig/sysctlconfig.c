/**
 *
 * File:        sysctlconfig.h
 * Description: Source file for configuring System clock and PLL's .
 *              Provides declarations for public functions and data types.
 * Date:        2025-11-09
 * Author:      Sagar Kokate
 * Copyright (c) 2025 Nalini Embedded Systems Pvt Ltd
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of Nalini Embedded systems Pvt Ltd
 * You shall not disclose such confidential information and shall use it only in
 * accordance with the terms of the license agreement you entered into with
 * Nalini Embedded systems Pvt Ltd.
 *
 */

/*********************************************Includes********************************************/
#include <stdint.h>
#include <stdbool.h>
#include "TMC4123gxl.h"
#include "sysctlconfig.h"

/************************************************MACROS*******************************************/
#define SYSCTL_SYSDIV_1                 0x00U
#define SYSCTL_SYSDIV_2                 0x01U
#define SYSCTL_SYSDIV_3                 0x02U
#define SYSCTL_SYSDIV_2_5               0x02U
#define SYSCTL_SYSDIV_4                 0x03U
#define SYSCTL_SYSDIV_5                 0x04U
#define SYSCTL_SYSDIV_6                 0x05U
#define SYSCTL_SYSDIV_7                 0x06U
#define SYSCTL_SYSDIV_8                 0x07U
#define SYSCTL_SYSDIV_9                 0x08U
#define SYSCTL_SYSDIV_10                0x09U
#define SYSCTL_SYSDIV_11                0x0AU
#define SYSCTL_SYSDIV_12                0x0BU
#define SYSCTL_SYSDIV_13                0x0CU
#define SYSCTL_SYSDIV_14                0x0DU
#define SYSCTL_SYSDIV_15                0x0EU
#define SYSCTL_SYSDIV_16                0x0FU
#define BYPASS                          (11U)
#define PWRDN                           (13U)
#define OSCSRC                          (4U)
#define PIOSC_DIV_BY_4                  (2U)
#define USESYSDIV                       (22U)
#define SYSDIV                          (23U)
#define PLLLMIS                         (0x6U)
#define PLLLRIS                         (0x6U)
#define MOSCDIS                         (0u)
#define USERCC2                         (31u)
#define DIV400                          (30u)
#define SYSDIV2LSB                      (22u)
#define PWRDN2                          (13u)
#define BYPASS2                         (11u)
#define SYSDIV2                         (23u)


#define MHZ_80                          (80u)
#define TRUE                            (1U)
#define FALSE                           (0U)

/*********************************************MACROS_PARAM****************************************/

/*********************************************Typedefs********************************************/


#if 0
typedef ClkPllConfig
{
    uint8_t u8_ByPassBit;   /* 1 = Bypass the PLL  */
    uint8_t SysClkDivisor;  
}ClkPllConfig_t;

#endif

typedef enum McuModes
{
    RUN_MODE,         /* ECU Run Mode */
    SLEEP_MODE,       /* ECU Sleep Mode */
    DEEPSLEEP_MODE,   /* ECU Deep Sleep Mode */
    HIBERNATE_MODE,   /* ECU Hibernation Mode */
    DYNAMIC_POWERMGMT /* ECU Dynamic Power Management mode */
} McuModes_t;

typedef struct ClkSleepConfig
{
    McuModes_t McuMode;
} ClkSleepConfig_t;



/***********************************************Export Object*************************************/
const Sysctlconfig_t g_sSysClkConfig[MAX_CLOCK_SRC] =
{
    {PIOSC, 16000000, 1},
    {PISOC_DIV_BY_4, 4000000, 0},
    {MOSC, 24000000, 1},
    {LFIOSC, 32768, 0},
    {HIB_MODULE_OSC, 32768, 0}
};

static Sysctlconfig_t s_GlobalClockConfig;

/*****************************************Export Functions**********************************/

/*******************************************************************************************/
uint8_t SysCtlClockConfig(ClockSrc_t eClockSource, uint8_t Is_PLLReq,uint8_t DsrcClk)
{
    uint8_t retval = E_NOT_OK;

    /* Function to configure system clock based on user configuration */
    Sysctlconfig_t *Ptr_clkConfig = &g_sSysClkConfig[eClockSource];

    if ((Ptr_clkConfig->IsPllUsed == 0) && (Is_PLLReq == TRUE))
    {
        retval = E_NOT_OK;
    }
    else
    {
        if (Ptr_clkConfig->eClockSrc < MAX_CLOCK_SRC)
        {
            /* clk Src is within the Src MAX */
            switch (eClockSource)
            {
            case PIOSC:
            case PISOC_DIV_BY_4:
            case MOSC:
            case LFIOSC:
            case HIB_MODULE_OSC:
            {
                retval = ClkConfig(Ptr_clkConfig, Is_PLLReq,DsrcClk);
                break;
            }
            default:
            {
                /* Default Clk PIOSC is Configured */
                break;
            }
            }
        }
        else
        {
            /* Do nothing  Wrong Clk Src Return with Default Clk Configurations */

            /* Update the GlobalClockConfig with Default Clock Setting Parameter */
        }
    }
    if (E_NOT_OK == retval)
    {
        /* Configure the Default Clock PISOC */
    }
    return retval;
}

/*****************************************************************************************************
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 ****************************************************************************************************/
uint8_t ClkConfig(Sysctlconfig_t *ClkPtr, uint8_t Is_PLLReq,uint8_t DsrcClk)
{
    uint8_t RetVal = E_OK;
    uint32_t u32_RCCreg = 0x00U;
    uint32_t u32_RCC2reg = 0x00U;
    uint32_t u32_legacyTime = 65535U;

    /* Read back current clock configurations */
    u32_RCCreg = RCC_REG;
    u32_RCC2reg = RCC2_REG;

    if (MHZ_80 == DsrcClk)
    {
        /* Enable RCC2 override */
        u32_RCC2reg |= (1U << USERCC2);

        /* Set BYPASS2 and clear USESYSDIV during setup */
        u32_RCC2reg |= (1U << BYPASS2);
        u32_RCCreg &= ~(1U << USESYSDIV);

        /* Select Oscillator Source in RCC2 (Bits 6:4) */
        u32_RCC2reg &= ~((1U << 4U) | (1U << 5U) | (1U << 6U));
        u32_RCC2reg |= (((uint32_t)(ClkPtr->eClockSrc) & 0x07U) << OSCSRC);

        /* Commit updates to physical registers */
        RCC_REG = u32_RCCreg;
        RCC2_REG = u32_RCC2reg;
    }
    else
    {
        /* Standard RCC Setup: Set BYPASS and clear USESYSDIV */
        u32_RCCreg |= (1U << BYPASS);
        u32_RCCreg &= ~(1U << USESYSDIV);

        /* Select Oscillator Source in RCC (Bits 5:4) */
        u32_RCCreg &= ~((1U << 4U) | (1U << 5U));
        u32_RCCreg |= (((uint32_t)(ClkPtr->eClockSrc) & 0x03U) << OSCSRC);

        /* Load RCC register */
        RCC_REG = u32_RCCreg;
    }

    switch(ClkPtr->eClockSrc)
    {
        case MOSC:
        case PIOSC:
        {
            if (MOSC == ClkPtr->eClockSrc)
            {
                /* Enable Main Osc */
                RCC_REG &= ~(1U << MOSCDIS);

                /* Clear and set 16 MHz XTAL value (0x15) */
                RCC_REG &= ~(0x1FU << 6U);
                RCC_REG |= (0x15U << 6U);
            }

            if (TRUE == Is_PLLReq)
            {
                /* Ensure PLL is powered ON (Clear PWRDN/PWRDN2 bits) */
                if (MHZ_80 == DsrcClk)
                {
                    RCC2_REG &= ~(1U << PWRDN2);
                }
                else
                {
                    RCC_REG &= ~(1U << PWRDN);
                }

                /* Clear PLL Lock Raw Interrupt Flag (Direct W1C) */
                MISC_REG = (1U << PLLLMIS);

                /* Poll for PLL Lock */
                while (!((RIS_REG >> PLLLRIS) & 0x01U))
                {
                    u32_legacyTime--;
                    if (0U == u32_legacyTime)
                    {
                        break;
                    }
                }
            }
            break;
        }
        case PISOC_DIV_BY_4:
        case LFIOSC:
        case HIB_MODULE_OSC:
        {
            RCC_REG &= ~(1U << USESYSDIV);
            RCC_REG &= ~(0x0FU << SYSDIV);
            break;
        }
        default:
        {
            RetVal = E_NOT_OK;
            break;
        }
    }

    /* Step 3: Apply system dividers and remove PLL bypass if lock succeeded */
    if (u32_legacyTime != 0U)
    {
        if (TRUE == Is_PLLReq)
        {
            if (MHZ_80 == DsrcClk)
            {
                /* Enable 400 MHz PLL output mode */
                RCC2_REG |= (1U << DIV400);

                /* Clear full 7-bit SYSDIV2 field (bits 28:22) */
                RCC2_REG &= ~(0x7FU << SYSDIV2);

                /* Set Divisor for 80 MHz (SYSCTL_SYSDIV_2_5 = 0x02U) */
                RCC2_REG |= (SYSCTL_SYSDIV_2_5 << SYSDIV2);

                /* Enable System Clock Divider in RCC */
                RCC_REG |= (1U << USESYSDIV);

                /* Unbypass PLL2 */
                RCC2_REG &= ~(1U << BYPASS2);
            }
            else
            {
                /* Enable System Clock Divider */
                RCC_REG |= (1U << USESYSDIV);

                /* Clear 4-bit SYSDIV field (bits 26:23) */
                RCC_REG &= ~(0x0FU << SYSDIV);

                /* Set 50 MHz Divider */
                RCC_REG |= (SYSCTL_SYSDIV_4 << SYSDIV);

                /* Unbypass PLL */
                RCC_REG &= ~(1U << BYPASS);
            }
        }
        RetVal = E_OK;
    }
    else
    {
        RetVal = E_NOT_OK;
    }

    return RetVal;
}

Sysctlconfig_t *GetCurrentClockConfigSettings(Sysctlconfig_t *g_sSysClkConfig)
{
    Sysctlconfig_t *ptr = &g_sSysClkConfig[g_sSysClkConfig->eClockSrc];

    /* Fill the ClkSrc and other parameters */

    return ptr;
}

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
#define SYSCTL_SYSDIV_1 0x00U
#define SYSCTL_SYSDIV_2 0x01U
#define SYSCTL_SYSDIV_3 0x02U
#define SYSCTL_SYSDIV_4 0x03U
#define SYSCTL_SYSDIV_5 0x04U
#define SYSCTL_SYSDIV_6 0x05U
#define SYSCTL_SYSDIV_7 0x06U
#define SYSCTL_SYSDIV_8 0x07U
#define SYSCTL_SYSDIV_9 0x08U
#define SYSCTL_SYSDIV_10 0x09U
#define SYSCTL_SYSDIV_11 0x0AU
#define SYSCTL_SYSDIV_12 0x0BU
#define SYSCTL_SYSDIV_13 0x0CU
#define SYSCTL_SYSDIV_14 0x0DU
#define SYSCTL_SYSDIV_15 0x0EU
#define SYSCTL_SYSDIV_16 0x0FU
#define BYPASS (11U)
#define PWRDN (13U)
#define OSCSRC (4U)
#define PIOSC_DIV_BY_4 (2U)
#define USESYSDIV (22U)
#define SYSDIV (23U)
#define PLLLMIS (0x6U)
#define PLLLRIS (0x6U)
#define MOSCDIS (0u)
#define TRUE (1U)
#define FALSE (0U)

/*********************************************MACROS_PARAM****************************************/

/*********************************************Typedefs********************************************/
typedef enum SysClockSrc
{
    PIOSC,          /* Precision Internal Oscillator */
    PISOC_DIV_BY_4, /* PIOSC divided by 4 */
    MOSC,           /* Main Oscillator */
    LFIOSC,         /* Low Frequency Internal Oscillator */
    HIB_MODULE_OSC, /* Hibernation Module Oscillator */
    MAX_CLOCK_SRC   /* Max Number of Src Clock */
} ClockSrc_t;

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

typedef struct Sysctlconfig
{
    ClockSrc_t eClockSrc;  /* Clk Source */
    uint32_t Clkfreq;      /* Desired Clk Frequency */
    bool IsPllUsed;        /* Is PLL required to System Clk */
    bool TypeOfOscillator; /* Internal or External Oscillator */
    bool Calibration;      /* Calibration is required */
    uint8_t CrystalVal;    /* Crystal Value for configuration */
    bool IsRunning;        /* Is the CLock is Running */
} Sysctlconfig_t;

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
uint8_t SysCtlClockConfig(ClockSrc_t eClockSource, uint8_t Is_PLLReq)
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
                retval = ClkConfig(Ptr_clkConfig, Is_PLLReq);
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
uint8_t ClkConfig(Sysctlconfig_t *ClkPtr, uint8_t Is_PLLReq)
{
    uint8_t RetVal = E_OK;
    uint32_t u32_RCCreg = 0x00U;
    uint32_t u32_RCC2reg = 0x00u;
    uint32_t u32_legacyTime = 65535U;

    /* Read back the Current Clk Configuration */
    u32_RCCreg = RCC_REG;
    u32_RCC2reg = RCC2_REG;

    /* Bypass the PLL and SysDiv and Supply Raw Clk */
    u32_RCCreg |= (1 << BYPASS);
    u32_RCCreg &= ~(1 << USESYSDIV);

    /* Select the Oscillator Source */
    u32_RCCreg &= ((uint32_t)~((1 << 4u) | (1 << 5u)));
    u32_RCCreg |= (((ClkPtr->eClockSrc) & 0x3u) << OSCSRC);

    /* Load the RCC value */
    RCC_REG = u32_RCCreg;

    switch(ClkPtr->eClockSrc)
    {
        case MOSC:
        case PIOSC:
        {
            if(MOSC = ClkPtr->eClockSrc)
            {
                /* Enable the Main Osc if its disabled */
                RCC_REG &= ((uint32_t)~(1<<MOSCDIS));
            }

            /* Clears the crystal bits */
            RCC_REG &= (uint32_t)(~(0x1Fu << 6u));

            /* Configure the Crystal Frequency Range 2 - 16 MHz */
            RCC_REG |= (uint32_t)(0x15u << 6u);

            if(TRUE == Is_PLLReq)
            {
                /* Clear the PLLRIS Bit */
                MISC_REG |= (1 << PLLLMIS);

                /* Check the PLL Status */
                while (!((RIS_REG >> PLLLRIS) & 0x01))
                {
                    /* Wait for Tready time for PLL */
                    u32_legacyTime--;

                    if (0U == u32_legacyTime)
                    {
                        break;
                    }
                }
            }
            else
            {

            }
            break;
        }
        case PISOC_DIV_BY_4:
        case LFIOSC:
        case HIB_MODULE_OSC:
        {
            /* Enable SystemClock Divider */
            RCC_REG &= (uint32_t)(~(1 << USESYSDIV));

            /* Clears the SYSDIV Bits */
            RCC_REG &= (uint32_t)(~(0xFu << SYSDIV));

            #if 0
            /* Use SysDiv to generate 50 Mhz */
            RCC_REG |= (SYSCTL_SYSDIV_4 << SYSDIV);
            #endif

            break;
        }
    }
    
    if (u32_legacyTime) // Tready time as per the Datasheet
    {
        if (Is_PLLReq)
        {
            /* Enable SystemClock Divider */
            RCC_REG |= (1 << USESYSDIV);

            RCC_REG &= (uint32_t)(~(0xFu << SYSDIV));

            /* Use SysDiv to generate 50 Mhz */
            RCC_REG |= (SYSCTL_SYSDIV_4 << SYSDIV);

            /* Disable the BYPASS Bit */
            RCC_REG &= ~(1 << BYPASS);
        }
        RetVal = E_OK;
    }
    else
    {
        RetVal = E_NOT_OK;
    }

    /* Update the Global Clock ConfigTree */

    return RetVal;
}

Sysctlconfig_t *GetCurrentClockConfigSettings(Sysctlconfig_t *g_sSysClkConfig)
{
    Sysctlconfig_t *ptr = &g_sSysClkConfig[g_sSysClkConfig->eClockSrc];

    /* Fill the ClkSrc and other parameters */

    return ptr;
}

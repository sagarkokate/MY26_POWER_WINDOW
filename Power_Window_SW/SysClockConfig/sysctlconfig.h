/**
 * 
 * File:        sysctlconfig.h
 * Description: Header file for the [clock configuration for the Desired Frequency].
 *              Provides declarations for public functions and data types.
 * Date:        2025-11-09
 * Author:      Sagar Kokate
 * Copyright (c) 2025 Nalini Embedded systems Pvt Ltd
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of Nalini Embedded systems Pvt Ltd
 * You shall not disclose such confidential information and shall use it only in
 * accordance with the terms of the license agreement you entered into with
 * Nalini Embedded systems Pvt Ltd.
 * 
 * 
 */
#ifndef SYSCTL_CONFIG_H
#define SYSCTL_CONFIG_H

/*********************************************Includes********************************************/




/*************************************************************************************************/



/************************************************MACROS*******************************************/







/*************************************************************************************************/



/*********************************************MACROS_PARAM****************************************/




/*************************************************************************************************/



/*********************************************Typedef****************************************/
typedef enum SysClockSrc
{
    PIOSC,          /* Precision Internal Oscillator */
    PISOC_DIV_BY_4, /* PIOSC divided by 4 */
    MOSC,           /* Main Oscillator */
    LFIOSC,         /* Low Frequency Internal Oscillator */
    HIB_MODULE_OSC, /* Hibernation Module Oscillator */
    MAX_CLOCK_SRC   /* Max Number of Src Clock */
} ClockSrc_t;

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

typedef enum SysClockSrc ClockSrc_t;
typedef struct Sysctlconfig Sysctlconfig_t;

/***********************************************Export Object*************************************/
extern Sysctlconfig_t * GetCurrentClockConfigSettings(Sysctlconfig_t *g_sSysClkConfig);
extern uint8_t ClkConfig(Sysctlconfig_t *ClkPtr, uint8_t Is_PLLReq,uint8_t DsrcClk);
extern uint8_t SysCtlClockConfig(ClockSrc_t eClockSource, uint8_t Is_PLLReq,uint8_t DsrcClk);
/*************************************************************************************************/





#endif  /* End of SYSCTL_CONFIG_H */

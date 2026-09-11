/**
 * 
 * File:        sysctlconfig.h
 * Description: Header file for the [Short description of module's purpose].
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
#ifndef TMC4123GXL_H
#define TMC4123GXL_H

/*********************************************Includes********************************************/




/*************************************************************************************************/


/******************************************Preprocessor Defines************************************
 * 
 **************************************************************************************************/
#define E_NOT_OK                    0x01U
#define E_OK                        0x00U


/************************************************MACROS*******************************************/

/* Define base addresses and offsets for System Control registers */
#define SYSTEM_CTLR_BASE_ADDR      0x400FE000U
#define RCC_OFFSET                 0x060U
#define RCC2_OFFSET                0x070U
#define MOSCCTL_OFFSET             0x07CU
#define DSLPCLKCFG_OFFSET          0x144U
#define SYSPROPERTIES_OFFSET       0x14CU
#define PIOSCCAL_OFFSET            0x150U
#define PIOSCSTAT_OFFSET           0x154U
#define PLLFREQ0_OFFSET            0x160U
#define PLLFREQ1_OFFSET            0x164U
#define PLLSTAT_OFFSET             0x168U
#define RIS_OFFSET                 0x50U
#define MISC_OFFSET                0x58U
#define RCC_REG                    (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + RCC_OFFSET))
#define RCC2_REG                   (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + RCC2_OFFSET))
#define MOSCCTL_REG                (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + MOSCCTL_OFFSET))
#define DSLPCLKCFG_REG             (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + DSLPCLKCFG_OFFSET))
#define SYSPROPERTIES_REG          (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + SYSPROPERTIES_OFFSET))
#define PIOSCCAL_REG               (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + PIOSCCAL_OFFSET))
#define PIOSCSTAT_REG              (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + PIOSCSTAT_OFFSET))
#define PLLFREQ0_REG               (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + PLLFREQ0_OFFSET))
#define PLLFREQ1_REG               (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + PLLFREQ1_OFFSET))
#define PLLSTAT_REG                (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + PLLSTAT_OFFSET))
#define RIS_REG                    (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + RIS_OFFSET))
#define MISC_REG                   (*(volatile uint32_t *)(SYSTEM_CTLR_BASE_ADDR + MISC_OFFSET))


/* Define Base Address and offsets for GPT Module */
#define TIMER0_BASE_ADDRESS         0x40030000U
#define TIMER1_BASE_ADDRESS         0x40031000U
#define TIMER2_BASE_ADDRESS         0x40032000U
#define TIMER3_BASE_ADDRESS         0x40033000U
#define TIMER4_BASE_ADDRESS         0x40034000U
#define TIMER5_BASE_ADDRESS         0x40035000U

#define WIDE_TIMER0_BASE_ADDRESS    0x40036000U
#define WIDE_TIMER1_BASE_ADDRESS    0x40037000U
#define WIDE_TIMER2_BASE_ADDRESS    0x4003C000U
#define WIDE_TIMER3_BASE_ADDRESS    0x4003D000U
#define WIDE_TIMER4_BASE_ADDRESS    0x4003E000U
#define WIDE_TIMER5_BASE_ADDRESS    0x4003F000U

#define GPTMCFG0_OFFSET             0x00000000U
#define GPTMTAMR0_OFFSET            0x00000004U
#define GPTMTBMR0_OFFSET            0x00000008U
#define GPTMCTL0_OFFSET             0x0000000CU
#define GPTMSYNC0_OFFSET            0x00000010U
#define GPTMIMR0_OFFSET             0x00000018U
#define GPTMRIS0_OFFSET             0x0000001CU
#define GPTMMIS0_OFFSET             0x00000020U
#define GPTMICR0_OFFSET             0x00000024U
#define GPTMTAILR0_OFFSET           0x00000028U
#define GPTMTBILR0_OFFSET           0x0000002CU
#define GPTMTAMATCHR0_OFFSET        0x00000030U
#define GPTMTBMATCHR0_OFFSET        0x00000034U
#define GPTMTAPR0_OFFSET            0x00000038U
#define GPTMTBPR0_OFFSET            0x0000003CU
#define GPTMTAPMR0_OFFSET           0x00000040U
#define GPTMTBPMR_OFFSET            0x00000044U
#define GPTMTAR0_OFFSET             0x00000048U
#define GPTMTBR0_OFFSET             0x0000004CU
#define GPTMTAV0_OFFSET             0x00000050U
#define GPTMTBV0_OFFSET             0x00000054U
#define GPTMRTCPD0_OFFSET           0x00000058U
#define GPTMTAPS0_OFFSET            0x0000005CU
#define GPTMTBPS0_OFFSET            0x00000060U
#define GPTMTAPV0_OFFSET            0x00000064U
#define GPTMTBPV0_OFFSET            0x00000068U
#define GPTMPP0_OFFSET              0x00000FC0U

#define GPTMCFG0                    (*(volatile uint32_t *)(TIMER0_BASE_ADDRESS + GPTMCFG0_OFFSET))
#define GPTMTAMR0                   (*(volatile uint32_t *)(TIMER0_BASE_ADDRESS + GPTMTAMR0_OFFSET))
#define GPTMTBMR0                   (*(volatile uint32_t *)(TIMER0_BASE_ADDRESS + GPTMTBMR0_OFFSET))
#define GPTMCTL0                    (*(volatile uint32_t *)(TIMER0_BASE_ADDRESS + GPTMCTL0_OFFSET))
#define GPTMSYNC0                   (*(volatile uint32_t *)(TIMER0_BASE_ADDRESS + GPTMSYNC0_OFFSET))
#define GPTMIMR0                    (*(volatile uint32_t *)(TIMER0_BASE_ADDRESS + GPTMCTL0_OFFSET))


/*************************************************************************************************/



/*********************************************MACROS_PARAM****************************************/




/*************************************************************************************************/



/***********************************************Typedef*******************************************/




/*************************************************************************************************/





/***********************************************Export Object*************************************/





/*************************************************************************************************/









/***********************************************Export Functions**********************************/





/*************************************************************************************************/





//***********************************************End of File****************************************/

#endif  
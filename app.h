/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef APP_H
#define APP_H
#include "types.h"
//#include "thor.h"
/**************************************************************************//**
 * @brief
 *    CMU initialization
 *****************************************************************************/
void
initCMU (void);
/**************************************************************************//**
 * @brief
 *    GPIO initialization
 *****************************************************************************/
void
initGPIO (void);

/**************************************************************************//**
 * @brief
 *    USART0 initialization
 *****************************************************************************/
void
initUSART0 (void);
void
check_gps (struct TGPS *GPS);
/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void
app_init (void);

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void
app_process_action (void);

#endif  // APP_H

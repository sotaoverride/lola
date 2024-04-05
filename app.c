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
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "thor.h"
#include "types.h"
// BSP for board controller pin macros
#include "bsp.h"
#include "misc.h"
// Size of the buffer for received data
#define BUFLEN  80
TGPS GPS;
/**************************************************************************//**
 * @brief
 *    CMU initialization
 *****************************************************************************/
void
initCMU (void)
{
  // Enable clock to GPIO and USART0
  CMU_ClockEnable (cmuClock_GPIO, true);
  CMU_ClockEnable (cmuClock_USART0, true);
}

/**************************************************************************//**
 * @brief
 *    GPIO initialization
 *****************************************************************************/
void
initGPIO (void)
{
  // Configure the USART TX pin to the board controller as an output
  GPIO_PinModeSet (BSP_BCC_TXPORT, BSP_BCC_TXPIN, gpioModePushPull, 1);

  // Configure the USART RX pin to the board controller as an input
  GPIO_PinModeSet (BSP_BCC_RXPORT, BSP_BCC_RXPIN, gpioModeInput, 0);

  /*
   * Configure the BCC_ENABLE pin as output and set high.  This enables
   * the virtual COM port (VCOM) connection to the board controller and
   * permits serial port traffic over the debug connection to the host
   * PC.
   *
   * To disable the VCOM connection and use the pins on the kit
   * expansion (EXP) header, comment out the following line.
   */
  GPIO_PinModeSet (BSP_BCC_ENABLE_PORT, BSP_BCC_ENABLE_PIN, gpioModePushPull,
                   1);
}

/**************************************************************************//**
 * @brief
 *    USART0 initialization
 *****************************************************************************/
void
initUSART0 (void)
{
  // Default asynchronous initializer (115.2 Kbps, 8N1, no flow control)
  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;

  // Route USART0 TX and RX to the board controller TX and RX pins
  GPIO->USARTROUTE[0].TXROUTE = (BSP_BCC_TXPORT
      << _GPIO_USART_TXROUTE_PORT_SHIFT)
      | (BSP_BCC_TXPIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[0].RXROUTE = (BSP_BCC_RXPORT
      << _GPIO_USART_RXROUTE_PORT_SHIFT)
      | (BSP_BCC_RXPIN << _GPIO_USART_RXROUTE_PIN_SHIFT);

  // Enable RX and TX signals now that they have been routed
  GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN
      | GPIO_USART_ROUTEEN_TXPEN;

  // Configure and enable USART0
  USART_InitAsync (USART0, &init);
  USART_BaudrateAsyncSet (USART0, 0, 9600, usartOVS16);
}

void
check_gps (TGPS *GPS)
{
  static uint64_t ModeTime = 0;
  static unsigned char Line[100];
  static int Length = 0;
  static int GPSFlightMode = 0;

  // SetFlightMode();

  while (1)
    {
      char Character;

      //Character = uart_getc(uart1);
      Character = USART_Rx (USART1);

      // putchar(Character);

      if (Character == '$')
        {
          Line[0] = Character;
          Length = 1;
        }
      else if (Length > 90)
        {
          Length = 0;
        }
      else if ((Length > 0) && (Character != '\r'))
        {
          Line[Length++] = Character;
          if (Character == '\n')
            {
              Line[Length] = '\0';
              printf ("%s", Line);
              ProcessLine (GPS, Line, Length);
              Length = 0;
            }
        }
    }

  if (get_time () > ModeTime)
    {
      int RequiredFlightMode;

      RequiredFlightMode = (GPS->Altitude > 1000) ? 6 : 3; // 6 is airborne <1g mode; 3=Pedestrian mode

      if (RequiredFlightMode != GPSFlightMode)
        {
          GPSFlightMode = RequiredFlightMode;

          SetFlightMode (RequiredFlightMode);
        }

      ModeTime = get_time () + 60000000L;
    }
}
/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void
app_init (void)
{
  initCMU ();
  initGPIO ();
  initUSART0 ();
  check_gps (&GPS);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void
app_process_action (void)
{
}

/*
 * thor.c
 *
 *  Created on: Apr 1, 2024
 *      Author: aatir
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#include "types.h"
#include "misc.h"
#include "thor.h"

#define LANDING_ALTITUDE    100

TGPS GPS;
Cords cords[350] =
  { 0 };
int cords_index = 0;
int32_t
find_distance (Cords c_1, Cords c_2)
{
  return (SQUARE(c_1.latitude - c_2.latitude)
      + SQUARE(c_1.longitude - c_2.longitude));
}

uint32_t
getTotalHeap (void)
{
  extern char __StackLimit, __bss_end__;

  return &__StackLimit - &__bss_end__;
}

uint32_t
getFreeHeap (void)
{
  struct mallinfo m = mallinfo ();

  return getTotalHeap () - m.uordblks;
}

int
GPSChecksumOK (unsigned char *Buffer, int Count)
{
  unsigned char XOR, i, c;

  XOR = 0;
  for (i = 1; i < (Count - 4); i++)
    {
      c = Buffer[i];
      XOR ^= c;
    }

  return (Buffer[Count - 4] == '*') && (Buffer[Count - 3] == Hex (XOR >> 4))
      && (Buffer[Count - 2] == Hex (XOR & 15));
}

void
FixUBXChecksum (unsigned char *Message, int Length)
{
  int i;
  unsigned char CK_A, CK_B;

  CK_A = 0;
  CK_B = 0;

  for (i = 2; i < (Length - 2); i++)
    {
      CK_A = CK_A + Message[i];
      CK_B = CK_B + CK_A;
    }

  Message[Length - 2] = CK_A;
  Message[Length - 1] = CK_B;
}

void
SendUBX (unsigned char *msg, int len)
{
  int i;

  for (i = 0; i < len; i++)
    {
      //uart_putc(uart1, msg[i]);
      USART_Tx (USART1, msg[i]);
    }
}

void
SetFlightMode (uint8_t NewMode)
{
  // Send navigation configuration command
  unsigned char setNav[] =
    { 0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00,
        0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC };

  setNav[8] = NewMode;

  FixUBXChecksum (setNav, sizeof(setNav));

  SendUBX (setNav, sizeof(setNav));

  printf ("Setting GPS flight mode %d\n", NewMode);
}

float
FixPosition (float Position)
{
  float Minutes, Seconds;

  Position = Position / 100;

  Minutes = trunc (Position);
  Seconds = fmod (Position, 1);

  return Minutes + Seconds * 5 / 3;
}

void
ProcessLine (TGPS *GPS, char *Buffer, int Count)
{
  float utc_time, latitude, longitude, hdop, altitude, speed, course;
  int lock, satellites, date;
  char active, ns, ew, units, speedstring[16], coursestring[16];

  if (GPSChecksumOK (Buffer, Count))
    {
      satellites = 0;

      if (strncmp (Buffer + 3, "GGA", 3) == 0)
        {
          if (sscanf (Buffer + 7, "%f,%f,%c,%f,%c,%d,%d,%f,%f,%c", &utc_time,
                      &latitude, &ns, &longitude, &ew, &lock, &satellites,
                      &hdop, &altitude, &units) >= 1)
            {
              // $GPGGA,124943.00,5157.01557,N,00232.66381,W,1,09,1.01,149.3,M,48.6,M,,*42
              GPS->Time = utc_time;
              GPS->Hours = GPS->Time / 10000;
              GPS->Minutes = (GPS->Time / 100) % 100;
              GPS->Seconds = GPS->Time % 100;
              GPS->SecondsInDay = GPS->Hours * 3600 + GPS->Minutes * 60
                  + GPS->Seconds;

              if (GPS->UseHostPosition)
                {
                  GPS->UseHostPosition--;
                }
              else if (satellites >= 4)
                {
                  GPS->Latitude = FixPosition (latitude);
                  if (ns == 'S')
                    GPS->Latitude = -GPS->Latitude;
                  GPS->Longitude = FixPosition (longitude);
                  if (ew == 'W')
                    GPS->Longitude = -GPS->Longitude;
                  GPS->Altitude = altitude;
                  if (cords_index == 0)
                    {
                      Cords tmp =
                        { .latitude = (int32_t) (GPS->Latitude * 1000000),
                            .longitude = (int32_t) (GPS->Longitude * 1000000) };
                      cords[cords_index] = tmp;
                      cords_index++;
                    }
                  else
                    {
                      Cords tmp =
                        { .latitude = (int32_t) (GPS->Latitude * 1000000),
                            .longitude = (int32_t) (GPS->Longitude * 1000000) };
                      /*
                       * 50 miles is 264,000 feet
                       * say you want to log only when the dist                                                             ance is more than 100 feet
                       * so it would be 100 / 264,000 = 0.000378
                       .000378 * 1000000 = 378
                       */
                      if (find_distance (tmp, cords[cords_index - 1])
                          >= (SQUARE(378)))
                        {
                          cords[cords_index] = tmp;
                          cords_index++;
                        }
                    }

                }

              GPS->Satellites = satellites;

              if (GPS->Altitude > GPS->MaximumAltitude)
                {
                  GPS->MaximumAltitude = GPS->Altitude;
                }

              if ((GPS->Altitude < GPS->MinimumAltitude)
                  || (GPS->MinimumAltitude == 0))
                {
                  GPS->MinimumAltitude = GPS->Altitude;
                }

              // Launched?
              if ((GPS->AscentRate >= 1.0)
                  && (GPS->Altitude > (GPS->MinimumAltitude + 150))
                  && (GPS->FlightMode == fmIdle))
                {
                  GPS->FlightMode = fmLaunched;
                  printf ("*** LAUNCHED ***\n");
                }

              // Burst?
              if ((GPS->AscentRate < -10.0)
                  && (GPS->Altitude < (GPS->MaximumAltitude + 50))
                  && (GPS->MaximumAltitude >= (GPS->MinimumAltitude + 2000))
                  && (GPS->FlightMode == fmLaunched))
                {
                  GPS->FlightMode = fmDescending;
                  printf ("*** DESCENDING ***\n");
                }

              // Landed?
              if ((GPS->AscentRate >= -0.1)
                  && (GPS->Altitude <= LANDING_ALTITUDE + 2000)
                  && (GPS->FlightMode >= fmDescending)
                  && (GPS->FlightMode < fmLanded))
                {
                  GPS->FlightMode = fmLanded;
                  printf ("*** LANDED ***\n");
                }
            }
        }
      else if (strncmp (Buffer + 3, "RMC", 3) == 0)
        {
          speedstring[0] = '\0';
          coursestring[0] = '\0';
          if (sscanf (Buffer + 7, "%f,%c,%f,%c,%f,%c,%[^','],%[^','],%d",
                      &utc_time, &active, &latitude, &ns, &longitude, &ew,
                      speedstring, coursestring, &date) >= 7)
            {
              // $GPRMC,124943.00,A,5157.01557,N,00232.66381,W,0.039,,200314,,,A*6C

              speed = atof (speedstring);
              course = atof (coursestring);

              GPS->Speed = (int) speed;
              GPS->Direction = (int) course;
            }
        }
      else if (strncmp (Buffer + 3, "GSV", 3) == 0)
        {
          // Disable GSV
          printf ("Disabling GSV\r\n");
          unsigned char setGSV[] =
            { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x01, 0x03, 0x39 };
          SendUBX (setGSV, sizeof(setGSV));
        }
      else if (strncmp (Buffer + 3, "GLL", 3) == 0)
        {
          // Disable GLL
          printf ("Disabling GLL\r\n");
          unsigned char setGLL[] =
            { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x01, 0x01, 0x2B };
          SendUBX (setGLL, sizeof(setGLL));
        }
      else if (strncmp (Buffer + 3, "GSA", 3) == 0)
        {
          // Disable GSA
          printf ("Disabling GSA\r\n");
          unsigned char setGSA[] =
            { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x01, 0x02, 0x32 };
          SendUBX (setGSA, sizeof(setGSA));
        }
      else if (strncmp (Buffer + 3, "VTG", 3) == 0)
        {
          // Disable VTG
          printf ("Disabling VTG\r\n");
          unsigned char setVTG[] =
            { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x01, 0x05, 0x47 };
          SendUBX (setVTG, sizeof(setVTG));
        }
      else
        {
          printf ("Unknown NMEA sentence: %s\n", Buffer);
        }
    }
  else
    {
      printf ("Bad checksum\r\n");
    }
}

/*
 void setup_gps(void)
 {
 // Set up I2C UBlox GPS
 printf("  - Init GPS  - ");

 // Initialise UART 1
 uart_init(uart1, 9600);
 gpio_set_function(4, GPIO_FUNC_UART);
 gpio_set_function(5, GPIO_FUNC_UART);

 printf("OK\n");
 }
 */

/*
 int main()
 {

 stdio_init_all();
 sleep_ms(3000);
 setup_default_uart();
 printf("THOR V1.0 \n");
 printf("\n========================\n\n");

 // Init modules
 printf("Initialisation ...\n\n");
 setup_gps();
 setup_prediction(&GPS);

 printf("\nTracker Running ...\n\n");

 while (1)
 {
 //    printf("\nTracker Running ...\n\n");
 check_gps(&GPS);
 //    check_prediction(&GPS);
 //    printf
 //    printf("hello %lu \n", getFreeHeap());
 }
 }
 */

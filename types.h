/*
 * types.h
 *
 *  Created on: Apr 1, 2024
 *      Author: aatir
 */

#ifndef TYPES_H_
#define TYPES_H_

#include "em_device.h"
#include "em_system.h"

typedef enum
{
  fmIdle, fmLaunched, fmDescending, fmLanding, fmLanded
} TFlightMode;

typedef struct
{
  long Time;      // Time as read from GPS, as an integer but 12:13:14 is 121314
  long SecondsInDay; // Time in seconds since midnight.  Used for APRS timing, and LoRa timing in TDM mode
  int Hours, Minutes, Seconds;
  float Longitude, Latitude;
  long Altitude, MinimumAltitude, MaximumAltitude, PreviousAltitude;
  unsigned int Satellites;
  int Speed;
  int Direction;
  float AscentRate;
  float BatteryVoltage;
  float InternalTemperature;
  float ExternalTemperature;
  float Pressure;
  float Humidity;
  TFlightMode FlightMode;
  float PredictedLongitude, PredictedLatitude;
  float CDA;
  int UseHostPosition;
  int TimeTillLanding;
  float PredictedLandingSpeed;
} TGPS;

typedef struct
{
  int32_t latitude;
  int32_t longitude;
} Cords;

#endif /* TYPES_H_ */

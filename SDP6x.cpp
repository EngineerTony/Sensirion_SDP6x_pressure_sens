/*
  SDP6x - A Low Pressor Sensor Library for Arduino.

  Supported Sensor modules:
    SDP600 series from Sensirion - http://www.futureelectronics.com/en/Technologies/Product.aspx?ProductID=SDP610125PASENSIRIONAG1050689&IM=0
  
  Created by Christopher Ladden at Modern Device on December 2009.
  Modified by Paul Badger March 2010
  Modified by www.misenso.com on October 2011:
  - code optimisation
  - compatibility with Arduino 1.0
  SDP6x Modified above by Antony Burness Jun 2016.
  - adapted for the I2C Pressure sensor
  - using example from sensirion https://www.sensirion.com/products/differential-pressure-sensors/all-documents-of-sensirions-differential-pressure-sensors-for-download/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/ 

#include <inttypes.h>
#include <Wire.h>
#include "Arduino.h"
#include "SDP6x.h"

#define SCALEFACTOR 240.0
// NOTE you will need to change the SCALEFACTOR to the appropriate value for your sensor
//  Don't forget the .0 at the end, it makes sure Arduino does not round the number
// SENSOR       | SDP6xx-500Pa and SDP5xx  |  SDP6x0-125Pa  |  SDP6x0-25Pa  |
// SCALEFACTOR  |         60.0             |     240.0      |     1200.0    | (1/Pa)

/******************************************************************************
 * Global Functions
 ******************************************************************************/

/**********************************************************
 * GetPressureDiff
 *  Gets the current Pressure Differential from the sensor.
 *
 * @return float - The Pressure in Pascal
 **********************************************************/
float SDP6xClass::GetPressureDiff(void)
{
  int16_t res;
  if (readSensor(ePresHoldCmd, (uint16_t*)&res)) {
	return ((float)(res)/SCALEFACTOR);
  } else {
	  return -10000000;
  }
}


/******************************************************************************
 * Private Functions
 ******************************************************************************/

bool SDP6xClass::readSensor(uint8_t command, uint16_t* res)
{
    uint16_t result;
	uint8_t data[2];
	uint8_t checksum;
	unsigned long startTime;
	
    Wire.beginTransmission(eSDP6xAddress);  //begin
    Wire.write(command);          //send the pointer location
    delay(100);
    Wire.endTransmission();                 //end

    Wire.requestFrom(eSDP6xAddress, 3);
	
	startTime = millis();
    while(Wire.available() < 3) {
		//wait 500ms for results.
		if (millis() - startTime > 500) {
			Wire.endTransmission();
			return false;
		}
    }

	//Store the result
	data[0] = Wire.read();
	data[1] = Wire.read();
	checksum = Wire.read();
	
	if (CheckCrc(data, 2, checksum) == NO_ERROR) {
		result = (data[0] << 8);
		result += data[1];
		*res = result; 
		return true;
	}
	
	return false;
}


void SDP6xClass::SetSensorResolution(etSensorResolutions resolution){
	uint16_t    userRegister; // advanced user register
	
	if (readSensor(eReadUserReg, &userRegister)) {
		userRegister &= 0xF1FF;
		userRegister |= (resolution & 0x07) << 9;
		writeSensor(userRegister);
	}

}


void SDP6xClass::writeSensor(uint16_t data){
	uint8_t bytes[2];

	bytes[0] = (data & 0xFF00) >> 8;
	bytes[1] = data & 0x00FF;
	
    Wire.beginTransmission(eSDP6xAddress);  //begin
    Wire.write(eWriteUserReg);          //send the pointer location
    Wire.write(bytes[0]);          //send the pointer location
    Wire.write(bytes[1]);          //send the pointer location
    delay(100);
    Wire.endTransmission();                 //end
}

//============================================================
// From sensirion App Note "CRC Checksum"
//calculates checksum for n bytes of data
//and compares it with expected checksum
//input: data[] checksum is built based on this data
// nbrOfBytes checksum is built for n bytes of data
// checksum expected checksum
//return: error: CHECKSUM_ERROR = checksum does not match
// 0 = checksum matches
//============================================================ 
PRES_SENSOR_ERROR SDP6xClass::CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum) {
	uint8_t crc = 0;
	uint8_t byteCtr;
	//calculates 8-Bit checksum with given polynomial
	for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr) { 
		crc ^= (data[byteCtr]);
		for (uint8_t bit = 8; bit > 0; --bit) { 
			if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
			else crc = (crc << 1);
		}
	}
	if (crc != checksum) return CHECKSUM_ERROR;
	else return NO_ERROR;
} 

SDP6xClass SDP6x;



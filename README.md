# Sensirion_SDP6x_pressure_sens
Arduino library to read pressure differences from the Sensirion SDP600 series low pressure sensors. (Uses I2C wire library)

## Where to buy the sensors:
Contact [Sensirion directly](https://www.sensirion.com/products/differential-pressure-sensors/digital-differential-pressure-sensors-without-zero-point-drift/)
Or check out [FutureElectronics](http://www.futureelectronics.com/en/Technologies/Product.aspx?ProductID=SDP610125PASENSIRIONAG1050689&IM=0)

## Using the files
Save all three files into a folder called SDP6x
Open the SDP6.ino file with Arduino (IDE a.k.a. the arduino coding program)
Select the correct Arduino device and port
Upload and enjoy.
Use Serial Monitor (or Ctrl+Shft+M) to see the output

## Using I2C
**SDP600 series sensors run on 3.3V!! not 5**

Don't worry the cool thing about I2C is that as long as the lowest voltage of any connected device (in this case the sensor with 3.3v) is higher than the Arduino's HIGH level (~2.5V) then it is ok to connect the sensor to the 3.3V pins and use 3.3V as the voltage for I2C.
This means that you can use the lower voltage sensors on a higher voltage Arduino.

JUST BE CAREFUL YOU DON'T CONNECT THE SENSOR TO 5V =D

The sensors use the wire (I2C) communication protocal so you will need to connect the SDA (data) and SCL (clock) wires to the correct analogue pins on your Arduino.
See [I2C/Wire](https://www.arduino.cc/en/Reference/Wire) library on Arcuino.cc for more information about which pins to use.
### NANO
On a NANO the pins are: 
* A4 for SDA
* A5 for SCL
* connect the PWR/+V sensor pin to 3.3V pin
* connect the GND/0V sensor pin to GND pin
* you should also connect a "pull up" resistor to the SDA and SCL connections
  * depending on the length of your connection wire 2.5k - 10k resistors should be ok
  * the "pull up" resistors connect the SCL and SDA pins to the +3.3V pin on your arduino
  * they will ensure that the SDA and SCL wires stay at HIGH (3.3V) when not in use making sure the sensors work properly
  * a 10uF capacitor between GND and 3.3V will also help smooth out any voltage bumps

### Other boards
* Uno
  * A4 (SDA)
  * A5 (SCL)
* Mega2560
  * 20 (SDA)
  * 21 (SCL)
* Leonardo
  * 2 (SDA)
  * 3 (SCL)
* Due
  * 20 (SDA) or SDA1
  * 21 (SCL) or SCL1
 
### Fritzing diagram of connections
![Connection Diagram](/I2C_sensor_connections.JPG)


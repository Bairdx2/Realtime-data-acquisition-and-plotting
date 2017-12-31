# Realtime-data-acquisiton-and-plotting

Arduino1---------------Arduino2---------------Raspi/computer---------------Realtime plots

##### ___I2C/wireless_____________pyserial______________________plot.ly
         
### Intended flow of the project:
  - Use a thermocouple and thermistor to measure temperature.
  - Use a load cell to measure weight or strain.
  - Aquire data from these sensors using an arduino.
  - Record data using the arduino.
  - Connect to a distant arduino using I2C or a simple wireless connection.
  - Have the second arduino connect to a Raspi or computer with Python via serial.
  - Using the pyserial library, collect the data from the second arduino.
  - Plot the data in real-time.



The project is written in modular chunks so that the user has the freedom to connect pieces as they choose in order
to achieve a more streamline version of the intended project or to help with hardware debugging along the way.


### Explanation of acronyms:
  - Ard = arduino
  - Pi = Raspberrypi
  - dR = change in resistance

### Explanation of files:
- Most arduino files have either two or three parts to them depending on how the two arduino can interact. Ex: the Temp_dR-I2C.ino
  is written for a thermistor and thermocouple which arduino 1 collects data for and it is connected to arduino 2 via I2C
  and running I2C_connection.ino.

- The Python files both assume a serial connection to arduino 2 but depending on how arduino 1 and 2 are connected (i2C or wireless)
  will determine which .py file is used. Ex: Pi-Ard_I2C_Plots.py assumes that arduino 1 and 2 are connected via I2C and the data is
  only plotted offline while Pi-Ard_I2C_Web_Plots.py does the same but streams/plots the data to [plot.ly](https://plot.ly/)
  
### Supplies used:
- Arduino UNO x2
- Raspberry Pi 3
- 10Kohm Negative TCR Thermistor
- Type K thermocouple
- [Adafruit Max31855 thermocouple amplifier](https://www.adafruit.com/product/269)
- [Loadcell amplifier](https://www.sparkfun.com/products/13879)
- [Loadcell](https://www.sparkfun.com/products/13329)
- [Adafruit datalogging shield](https://www.adafruit.com/product/1141)
- [315MHz RF transmitter](https://www.sparkfun.com/products/10535)
- [315MHz RF receiver](https://www.sparkfun.com/products/10533)

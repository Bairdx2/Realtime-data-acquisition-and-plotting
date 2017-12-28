#Realtime-data-acquisiton-and-plotting

Intended flow of the project:
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

# Script: Pi-Ard_wireless_Plots.py
# Description: This script takes a stream of serial input from an arduino
#              containing temperature, resistance, and weight data.
#              The data is then plotted in the using matplotlib.
# 
# Author: Kevin Rivera
# Creation date: 12/08/2017
# Version: v1.0

######################################### Imports ##############################################

import serial as S                           # serial library is imported
import matplotlib.pyplot as plt


######################################### Variables ############################################

R = ''                                      # Thermistor data extracted from a message will be kept here temporarily.
T = ''                                      # Thermocouple data extracted from a message will be kept here temporarily.
W = ''                                      # Weight data extracted from a message will be kept here temporarily.
prev = ''                                   # First character of the previous extracted message kept here temporarily.
thermistor = []                             # Thermistor data will be appended to this list.
typeK = []                                  # Thermocouple data will be appended to this list.
weight = []                                 # Weight data will be appended to this list.
timeT = []                                  # Time for the temperature plots will be appended to this list.
timeW = []                                  # Time for the weight plots will be appended to this list.
tickT = 0                                   # Counter for the temperature time data.
tickW = 0                                   # Counter for the weight time data.
com1 = 0                                    # Counter to keep track of comma 1.
com2 = 0                                    # Counter to keep track of comma 2.


######################################### Body #################################################

port = S.Serial('COM5',9600)         		    # Port is opened to allow communication
                                                # between the rasPi and arduino
											 
for i in range(20):
    message = port.readline() 				    # Message from the arduino is read
    message = message.decode()
    if message[0] == 'a':                       # Check to see if the message is thermocouple data
        k = 0
        while(k < len(message)):                # The two resistance values are pulled from the
            if(message[k] == ','):              # decoded message.
                com1 = k;
                R = message[1:k]
                T = message[k+1:]
                thermistor.append(float(R))
                typeK.append(float(T))
                break                           # The while loop is exited once the values are obtained.
            k+=1
        tickT = tickT + 1
        timeT.append(tickT)
            
    if message[0] == 'b' and prev == 'a':       # Check to see if the message is thermistor related
        W = message[1:]
        weight.append(float(W))
        tickW = tickW + 1
        timeW.append(tickW)
                
    prev = message[0]
    
port.close()

# Temperature plot
fig, ax1 = plt.subplots()

ax1.plot(timeT, thermistor, 'b-')
ax1.set_xlabel('time (s)')
# Make the y-axis label, ticks and tick labels match the line color.
ax1.set_ylabel('Resistance', color='b')
ax1.tick_params('y', colors='b')
ax2 = ax1.twinx()
ax2.plot(timeT, typeK, 'r--')
ax2.set_ylabel('Temperature(degC)', color='r')
ax2.tick_params('y', colors='r')
fig.tight_layout()
plt.grid()
plt.show()

# Weight plot
plt.figure()
plt.plot(timeW,weight)
plt.xlabel('Time(s)')
plt.ylabel('Pounds(lbs)')
plt.grid()
plt.show()
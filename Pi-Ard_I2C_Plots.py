# Script: Pi-Ard_I2C_Plots.py
# Description: This script takes a stream of serial input from an arduino
#              containing temperature, resistance, and weight data.
#              The data is then plotted in the using matplotlib. The ability
#              to live stream data is possible but follow the instructions
#	       below to do so.
# 
# Author: Kevin Rivera
# Creation date: 12/08/2017
# Version: v1.0

######################################### Imports ##############################################

import serial as S                           
import matplotlib.pyplot as plt
#import plotly as pl
#from plotly.graph_objs import Scatter, Layout, Figure
#import plotly.plotly as py


######################################### Variables ############################################


R = ''                                      # Thermistor data extracted from a message will be kept here temporarily.
T = ''                                      # Thermocouple data extracted from a message will be kept here temporarily.
W = ''                                      # Weight data extracted from a message will be kept here temporarily.
prev = ''                                   # First character of the previous extracted message kept here temporarily.
thermistor = []                             # Thermistor data will be appended to this list.
typeK = []                                  # Thermocouple data will be appended to this list.
weight = []                                 # Weight data will be appended to this list.
time = []                                   # Time for the plots will be appended to this list.
commas = []                                 # Index positions for commas will be appended to this list temporarily.
tick = 0                                    # Counter for the plots time data.

# Uncomment this code and fill the values in order to live stream the data to plot.ly

#username = 'username'
#api_key = 'key'
#stream_token1 = 'token 1'
#stream_token2 = 'token 2'
#py.sign_in(username, api_key)

#trace1 = Scatter(x=[],y=[],stream=dict(token=stream_token1,maxpoints=200))
#layout = Layout(title='Raspberry Pi Streaming Sensor Data')
#fig = Figure(data=[trace1], layout=layout)
#py.plot(fig, filename='Raspberry Pi Streaming Example Values')

#stream1 = py.Stream(stream_token1)
#stream2 = py.Stream(stream_token2)
#stream2.open()
#stream1.open()


######################################### Body #################################################

port = S.Serial('COM5',9600)         		   # Port is opened to allow communication.
                                            # between the rasPi and arduino.
											 
for i in range(60):
#while True:
    message = port.readline() 				   # Message from the arduino is read.
    message = message.decode()
    k = 0
    while(k < len(message)):                # The two resistance values are pulled from the
        if(message[k] == ','):              # decoded message.
            commas.append(k)
        k+=1
    W = message[0:commas[0]]
    R = message[commas[0]+1:commas[1]]
    T = message[commas[1]+1:]
    thermistor.append(float(R))
    typeK.append(float(T))
    weight.append(float(W))
    
    #stream1.write({'x': tick, 'y': R})     # Streams resistance or temperature data to plot.ly
    #stream2.write({'x': tick, 'y': T})     # Only uncomment one of these at a time.
    
    tick = tick + 1
    time.append(tick)
    commas = []
    
port.close()


fig, ax1 = plt.subplots()

# Temperature plot
ax1.plot(time, thermistor, 'b-')
ax1.set_xlabel('time (s)')
ax1.set_ylabel('Resistance', color='b')
ax1.tick_params('y', colors='b')
ax2 = ax1.twinx()
ax2.plot(time, typeK, 'r--')
ax2.set_ylabel('Temperature(degC)', color='r')
ax2.tick_params('y', colors='r')
fig.tight_layout()
plt.grid()
plt.show()

# Weight plot
plt.figure()
plt.plot(time,weight)
plt.xlabel('Time(s)')
plt.ylabel('Pounds(lbs)')
plt.grid()
plt.show()

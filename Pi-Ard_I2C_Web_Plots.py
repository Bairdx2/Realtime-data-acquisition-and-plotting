# Script: Pi-Ard_I2C_Web_Plots.py
# Description: This script takes a stream of serial input from an arduino
#              containing temperature, resistance, and weight data.
#              The data is then transmitted to plot.ly to be plotted
#              and updated in realtime. The data is also plotted using
#              matplotlib as an offline option.
# 
# Author: Kevin Rivera
# Creation date: 12/08/2017
# Version: v1.0

######################################### Imports ##############################################

import serial as S                           
import matplotlib.pyplot as plt
import plotly as pl
from plotly.graph_objs import Scatter, Layout, Figure
import plotly.plotly as py


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

username = 'Bairdx2'                        # Username for plot.ly.
api_key = 'SRO19YUo5EP4VtrMqega'            # API key provided by plot.ly.
stream_token1 = 'r0jkx2ynjq'                # Stream token provided by ploy.ly.
stream_token2 = '7l1fleh3cj'
stream_token3 = 'vmhuj53h2x'


stream1=dict(token=stream_token1,maxpoints=1000)# Keep the stream token and number of data points you want visible on the screen at once. 
stream2=dict(token=stream_token2,maxpoints=1000)
stream3=dict(token=stream_token3,maxpoints=1000)


py.sign_in(username, api_key)				# Sign in to the plot.ly account to plot data.

Resistance = Scatter(x=[], y=[], stream=stream1, name='Resistance', marker = dict(color = 'rgb(0,0,255)'))
Temperature = Scatter(x=[], y=[], stream=stream2, yaxis='y2', name='Temperature', marker=dict(color='rgb(255, 0, 0)'))
Weight = Scatter(x=[], y=[], stream=stream3, name='Weight(lbs)', marker = dict(color = 'rgb(0,0,255)'))
#rgb(148, 103, 189)
data1 = [Resistance, Temperature]
data2 = [Weight]

layout1 = Layout(					# Formatting plots to be double axis for temperature data.
    title = 'Temperature and resistance data',
    yaxis = dict(
        title = 'Resistance(ohms)',
        titlefont = dict(
            color = 'rgb(0, 0, 255)'
        ),
        tickfont = dict(
            color = 'rgb(0, 0, 255)'
        ),
    ),
    xaxis = dict(
        title = 'Time(s)'
    ),
    yaxis2 = dict(
        title = 'Temperature(degC)',
        titlefont = dict(
            color ='rgb(255, 0, 0)'
        ),
        tickfont = dict(
            color ='rgb(255, 0, 0)'
        ),
        overlaying = 'y',
        side = 'right'
    )
)
        
layout2 = Layout(					# Formatting for the weight plot
    title='Load cell data',
    yaxis = dict(
        title = 'Weight(lbs)',
        titlefont = dict(
            color = 'rgb(0, 0, 255)'
        ),
        tickfont = dict(
            color = 'rgb(0, 0, 255)'
        ),   
    ),
    xaxis = dict(
        title = 'Time(s)'
    )            
)

fig1 = Figure(data=data1, layout=layout1)
py.plot(fig1, filename='Temperature and resistance data',auto_open=False)# Name of the file that will be saved on plot.ly

fig2 = Figure(data=data2, layout=layout2)
py.plot(fig2, filename='Load cell data',auto_open=False)

stream1 = py.Stream(stream_token1)				# Initializes the streaming of data to the plot.ly
stream2 = py.Stream(stream_token2)
stream3 = py.Stream(stream_token3)

stream1.open()							# Opens a window with the plots formatted above.
stream2.open()
stream3.open()


######################################### Body #################################################

port = S.Serial('COM5',9600)         		# Port is opened to allow communication
                                                # between the rasPi and arduino.
											 
for i in range(300):

    #while True:
    message = port.readline() 		        # Message from the arduino is read.
    message = message.decode()
    k = 0
    while(k < len(message)):                    # The two resistance values are pulled from the
        if(message[k] == ','):                  # decoded message.
            commas.append(k)
        k+=1
    W = message[0:commas[0]]                    # Data is converted to rational numbers to the 
    R = message[commas[0]+1:commas[1]]          # and appended to the lists here.
    T = message[commas[1]+1:]
    thermistor.append(float(R))
    typeK.append(float(T))
    weight.append(float(W))
    
    stream1.write({'x': tick, 'y': R})          # Data is streamed to the plot.ly site
    stream2.write({'x': tick, 'y': T})
    stream3.write({'x': tick, 'y': W})

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

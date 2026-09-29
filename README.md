# Wearable Smart Helmet with Fall Detection using IoT

## Overview

Wearable Smart Helmet is an IoT-based safety project designed to improve two-wheeler rider safety. The system detects alcohol consumption and sudden falls, controls the vehicle ignition, and provides alerts during unsafe conditions.

## Technologies Used

* NodeMCU ESP8266
* Embedded C
* Alcohol Sensor
* Tilt Sensor
* Relay Module
* Buzzer
* I2C LCD Display
* IoT Communication

## Features

* Alcohol detection
* Fall and accident detection
* Automatic engine ignition control
* Buzzer alert during accidents
* IoT-based emergency alerts
* Real-time LCD status display
* Continuous safety monitoring

## Working

The alcohol sensor detects the presence of alcohol and sends the readings to the NodeMCU. If the alcohol level exceeds the predefined threshold, the relay prevents the engine from starting.

The tilt sensor detects sudden falls or accident conditions. When an accident is detected, the buzzer is activated and an alert is sent through IoT connectivity. The LCD displays the current system status.

## Future Enhancements

* GPS-based accident location tracking
* GSM-based emergency alerts
* Mobile application for monitoring
* Health monitoring sensors
* Camera-based accident recording
* AI-based accident prediction


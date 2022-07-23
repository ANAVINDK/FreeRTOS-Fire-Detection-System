# FreeRTOS-Fire-Detection-System

## Microcontroller Boards
1. Arduino Mega2560

## Sensors
1. Fire sensor
2. Smoke sensor - MQ 2
3. IR sensor 

## Actuators
1. Cooler Fan (DC motor)
2. Buzzer
3. LEDs
4. Relays


## Descriptions

Real-time Fire Detection system that we implemented works on the basis of sensing values from different nodes like smoke detection node ,flame detection node and IR detection node , these values are monitored periodically and any variation from threshold value will activate the corresponding actuation(buzzer, sprinkler, cooler, led blinking) to maintain the condition stable and for indicating purpose .

## Software Implementation

The software program for this project is written in Arduino IDE. Our project uses FreeRTOS real time operating system. FreeRTOS is a market-leading real-time operating system (RTOS) for microcontrollers and small microprocessors. FreeRTOS library of Arduino provides suitable function for creating a task, delaying a task and deleting traces of tasks etc. The priority of each task is assigned in xtaskCreate function and a vtaskDelay function is used for enabling repetition of particular task at regular interval which denotes periodicity. The actuation tasks are initially at a blocked state. Until the sensing task sends a notification indicating presence of fire or smoke these tasks will be in blocked state and cannot come into the system. This helps in reduced CPU utilization and resource conservation. This method also provide efficiency in using RAM. Our program consists of five functions defining five tasks. Handlers are used to change the state of actuation tasks from blocking through sensing tasks. Two handlers are used each for flame task and smoke task.


## Hardware Implementation

The hardware is assembled on Arduino Mega2560 board. This is a powerful board which runs on ATmega2560 microcontroller. This board contains 54 digital input/output pins and a 16 MHz crystal oscillator which makes the board suitable for this project. Flame detected using flame sensor which is commonly available. Smoke sensor used for this project is MQ-2 sensor which has good accuracy detecting smoke, LPG etc. An infrared (IR) sensor is an electronic device that measures and detects infrared radiation in its surrounding environment. We are using an IR sensor to detect a presence of human movement. The actuation is done through a relay, cooler and an alarm. 

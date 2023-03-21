# Arduino-CSGO-Bomb

## Description
Functioning CSGO Bomb using Arduino. Meant to be used with nerfs or softguns.

Components : 
- Arduino UNO 
- I2C LED screen
- 4x3 matrix keypad 
- 2 buzzers
- 3 Leds (R, Y, G)

## Functioning : 
To input a 4-digit code, press *
To confirm a code, press #

### Stage 1:
Bomb is in standby state. Input code 0001 to start a game. 

### Stage 2 : 
Game started
- 3 minutes timer on the LED screen
- Yellow LED flashing
- Accelerating buzzing sound

Input code 1739 to plant the bomb 

### Stage 3 : 
Bomb is planted
- 1 minute timer on LED screen
- Red LED flashing
- Accelerating buzzing sound 

Input code 2486 to defuse the bomb

### Stage 4 : 
Bomb is defused
Green LED is on

### Stage 5 : 
Bomb exploded
Red LED is on

# Arduino-CSGO-Bomb

## Description
Functioning CSGO Bomb using Arduino. Meant to be used with nerfs or softguns.

Copy and paste diagram.json in https://wokwi.com > start from scrath > diagram.json to view circuit diagram and wiring

Components : 
- Arduino UNO 
- I2C 1602 LCD screen
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
- Plant timer starts counting down
- Yellow LED flashing
- Accelerating buzzing sound

Input code 1739 to plant the bomb 

### Stage 3 : 
Bomb is planted
- Defuse timer starts counting down
- Red LED flashing
- Accelerating buzzing sound 

Input code 2486 to defuse the bomb

### Stage 4 : 
Bomb is defused
Green LED is on
Input code 0009 to reset game and go back to stage 1

### Stage 5 : 
Bomb exploded
Red LED is on
Input code 0009 to reset game and go back to stage 1

### Utilities : 
Input code 0005 at any time during the game to pause
Input code 0009 at any time during the game to end it and go back to stage 1
Input code 1111 in standby mode to edit timer values

### Default values : 
- Plant timer : 3 minutes
- Defuse timer : 3 minutes 
- Codes :
-     char armPass[4] = { '1', '7', '3', '9' };
-     char defPass[4] = { '2', '4', '8', '6' };
-     char editPass[4] = { '1', '1', '1', '1' };
-     char initPass[4] = { '0', '0', '0', '0' };
-     char begPass[4] = { '0', '0', '0', '1' };
-     char pauseTimePass[4] = { '0', '0', '0', '5' };
-     char endPass[4] = { '0', '0', '0', '9' };

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

Arduino libraries : 
- LiquidCrystal_I2C.h
- Keypad.h


## Functioning : 
- To input a 4-digit code, press *
- To confirm a code, press #

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
Input code 0000 to reset all values to default
Input code 0005 at any time during the game to pause
Input code 0009 at any time during the game to end it and go back to stage 1
Input code 1111 in standby mode to edit timer values

### Default values : 
- Plant timer : 3 minutes
- Defuse timer : 3 minutes 

|Function        | Code |
|----------------|------|
|Plant the bomb  | 1739 |
|Defuse the bomb | 2486 |
|Edit timers     | 1111 |
|Reset values    | 0000 |
|Start game      | 0001 |
|Pause game      | 0005 |
|End game        | 0009 |

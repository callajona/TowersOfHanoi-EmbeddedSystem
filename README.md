# TowersOfHanoi-EmbeddedSystem
Towers of Hanoi game created for an embedded systems module - 2nd year Electronic and Electrical Engineering @ University of Leeds, UK.

## Required Technology
- Nucleo Board: [STM32L476RG](https://www.st.com/en/evaluation-tools/nucleo-l476rg.html)
- LCD Screen: [Nokia 5110](https://wiki.geeetech.com/index.php/Nokia_5110_LCD_Module)
- Joystick (Dual-axis w/ button - typically 5-pin)
- LED (and appropriate current limiting resistor)
- Jumper wires for connections
- Breadboard

## Required Libraries
- [Joystick Driver](https://github.com/ELECXJEL2645/N5110)
- [N5110 LCD Driver](https://github.com/ELECXJEL2645/N5110)

## Video Demo
[Video Demo](https://youtu.be/2UclU0JsHCk)

## Game Rules
There are three pegs and a tower of concentrically smaller discs is constructed on one of the pegs. 
The objective is to move the entire tower from one peg to another, but you can only move one disc at a time and you cannot place a larger disc ontop of a smaller disc. 

_Note: All the code is in one file, as the concept of separating different sections hadn't fully sunk in yet. Apologies for 900 lines of code in one file and for a lack of commenting at points._

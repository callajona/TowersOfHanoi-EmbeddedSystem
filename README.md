# Towers of Hanoi - Code (for Terminal Game & Embedded System)
Towers of Hanoi game created for an embedded systems project in my 2nd year Electronic and Electrical Engineering degree @ University of Leeds, UK.
Two versions:
- Terminal Game: Representation of game displayed in the terminal, moves entered using keyboard
- Embedded System: Peripheral I/O devices used to display the game (LCD screen) and move the discs (joystick) 

NOTE: Due to Arm terminating the MBed OS and Platform, the emdedded system code will require adapting to run on a different platform. The core mechanics of the code (the Towers of Hanoi game itself - as shown in the terminal version) should remain functional as it was developed in isolation of Mbed, but how the system interacts with the I/O devices is likely to require adaptation. 

## Hardware
- Nucleo Board: [STM32L476RG](https://www.st.com/en/evaluation-tools/nucleo-l476rg.html)
- LCD Screen: [Nokia 5110](https://wiki.geeetech.com/index.php/Nokia_5110_LCD_Module)
- Joystick (Dual-axis w/ button - typically 5-pin)
- LED (and appropriate current limiting resistor)
- Jumper wires for connections
- Breadboard

## Software
Programmed using [Mbed](https://os.mbed.com/)
- Keil Studio Cloud (Online Compiler)
- Mbed Studio (Desktop)

### Required Libraries
- [Joystick Driver](https://github.com/ELECXJEL2645/N5110)
- [N5110 LCD Driver](https://github.com/ELECXJEL2645/N5110)
- [MBed OS](https://github.com/ARMmbed/mbed-os/tree/master)

## Project Information
### Game Rules
There are three pegs and a tower of concentrically smaller discs is constructed on one of the pegs. 
The objective is to move the entire tower from one peg to another, but you can only move one disc at a time and you cannot place a larger disc ontop of a smaller disc. 

### Video Demo
[Video Demo](https://youtu.be/2UclU0JsHCk)

_Note: All the code is in one file, as the concept of separating different sections hadn't fully sunk in yet. Apologies for 900 lines of code in one file and for a lack of commenting at points._

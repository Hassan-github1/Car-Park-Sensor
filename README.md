# Car-Park-Sensor

## Project Overview

This project showcases a Car Park Sensor system using a PIC16F877A microcontroller. The system employs four ultrasonic sensors to measure the distance to 
nearby obstacles and provides feedback via an LCD display, a buzzer, and a progress bar made of LEDs. This project simulates the behavior of car parking 
sensors, enhancing safety and ease of parking.

## Features

- **Distance Measurement:** Measures distance to obstacles using four ultrasonic sensors.
- **LCD Display:** Real-time display of distance measurements for each sensor.
- **Progress Bar:** Visual representation of distance using LEDs.
- **Buzzer Alert:** Audible alerts with varying frequencies based on distance.

## Components

- PIC16F877A Microcontroller
- HC-SR04 Ultrasonic Sensors (4x)
- 16x2 LCD Display
- Buzzer
- LEDs (for progress bar)
- Resistors
- Breadboard and Jumper Wires
- Power Supply

## Pin Configuration

- **LCD:**
  - RS -> RD2
  - EN -> RD3
  - D4 -> RD4
  - D5 -> RD5
  - D6 -> RD6
  - D7 -> RD7
- **Ultrasonic Sensors:**
  - Trigger1 -> RC2
  - Echo1 -> RC3
  - Trigger2 -> RC4
  - Echo2 -> RC5
  - Trigger3 -> RC6
  - Echo3 -> RC7
  - Trigger4 -> RB5
  - Echo4 -> RB4
- **Buzzer:** RD0
- **Progress Bar LEDs:** PORTB

## Circuit Diagram

![image](https://github.com/Hassan-github1/Car-Park-Sensor/assets/173414971/8a65eb35-79f7-4e73-8d66-df6728527337)


## Getting Started

### Hardware Setup

1. **Connect the Components:**
   - Connect the ultrasonic sensors, LCD, buzzer, and LEDs to the PIC16F877A microcontroller as per the pin configuration.
   - Use a breadboard and jumper wires for making the connections.
   - Ensure proper power supply to the circuit.

2. **Verify Connections:**
   - Double-check the connections to ensure all components are correctly wired.

### Software Setup

1. **Install MPLAB X IDE and XC8 Compiler:**
   - Download and install MPLAB X IDE from the [Microchip website](https://www.microchip.com/mplab/mplab-x-ide).
   - Install the XC8 compiler for compiling the code.

2. **Open the Project:**
   - Open MPLAB X IDE and create a new project.
   - Copy the provided code (`main.c`) into your project.

3. **Compile and Upload:**
   - Compile the code using the XC8 compiler.
   - Upload the compiled code to the PIC16F877A microcontroller using a PICkit3 or any compatible programmer.

## Usage

1. **Power Up:**
   - Power up the circuit. The LCD will display a welcome message, indicating successful initialization.

2. **Distance Measurement:**
   - The system will start measuring distances using the four ultrasonic sensors.
   - Distances will be displayed on the LCD in real-time.

3. **Visual and Audible Feedback:**
   - The progress bar made of LEDs will light up based on the distance measured by the first sensor.
   - The buzzer will emit sounds with varying frequencies based on the distance, providing an audible alert.

## Project Files

- **main.c:** Contains the main code for the project.

## Function Descriptions

### LCD Functions

- **Lcd_SetBit(char data_bit):** Sets the data bits for the LCD.
- **Lcd_Cmd(char a):** Sends commands to the LCD.
- **Lcd_Clear(void):** Clears the LCD screen.
- **Lcd_Set_Cursor(char a, char b):** Sets the cursor position on the LCD.
- **Lcd_Start(void):** Initializes the LCD.
- **Lcd_Print_Char(char data):** Prints a single character to the LCD.
- **Lcd_Print_String(char *a):** Prints a string to the LCD.

### Distance Calculation Functions

- **calc_dist1(void):** Calculates the distance from the first ultrasonic sensor.
- **calc_dist2(void):** Calculates the distance from the second ultrasonic sensor.
- **calc_dist3(void):** Calculates the distance from the third ultrasonic sensor.
- **calc_dist4(void):** Calculates the distance from the fourth ultrasonic sensor.

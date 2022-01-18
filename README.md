# Intelligent Seasoner
**A Device that Provides You with Desired Water.**

## Demonstration
Online Demo Video: https://www.youtube.com/watch?v=U9WBdGgQuSw

## Motivation
Given a drink, it’s hard to adjust the concentration manually.

We want some drinks with specific concentration.

## Features
- Internet of Things: Bluetooth Control.
- Real-time precise and stable control.
- No calculations need for customers.
- Set up your favorite favor anytime.

## Measurement Data of Water
| | Temperature (°C) | TDS Value (ppm) |
| :--- | ---: | ---: |
| Pure Water | 19.6 | 10 |
| Mineral Water | 19 | 180 |
| Branded-Mineral Water | 19.5 | 337 |

## Main Structure
<img src="https://user-images.githubusercontent.com/64905288/149810651-cfbda81c-51a2-40aa-8837-48cb425177f4.png" width="500" />

## Operation Modes
**3 different modes: Full Mode, Works without Computer, Works without Computer and Phone**
### Full Mode
#### Features
- Connect to Phone via Bluetooth
- Start/Stop by User Button on STM32 or Phone Tune Parameters by Phone
  - Customized Target TDS Value
  - Customized PID Control Parameters
- Display instant TDS Value and Graph on the screen
#### Structure
<img src="https://user-images.githubusercontent.com/64905288/149809874-3a2b0425-8079-4ab2-924a-71a484acf29a.png" width="500" />

### Works without Computer
#### Features
- Connect to Phone via Bluetooth
- Start/Stop by User Button on STM32 or Phone Tune Parameters by Phone
  - Customized Target TDS Value
  - Customized PID Control Parameters
#### Structure
<img src="https://user-images.githubusercontent.com/64905288/149810131-c1f77f73-4ae8-4204-9ce3-f16b244dbcfa.png" width="500" />

### Works without Computer and Phone
#### Features
- Start/Stop by User Button on STM32
- Use onboard default parameters
- Parameters with fine-tuned values
#### Structure
<img src="https://user-images.githubusercontent.com/64905288/149810459-5d68d21d-87a1-406d-8fc8-2604f60defe7.png" width="500" />

## Technology Used

### Software
- Real-time Feedback PID control
- BLE Programming

### Embedded
- Multi-Thread Programming
- PWM Servo control
- TDS Meter
  - Analog to Digital Input
- Real-time OS Interrupt
- Programming I/O

### Mechanism
- Mechatronics
- Valves with Robotic Arm and Claw Control
<img src="https://user-images.githubusercontent.com/55504676/149816906-5d97a99a-ab66-48c2-8fe2-cd04c4397644.png" width="500" />

## Programming Model
<img src="https://user-images.githubusercontent.com/55504676/149817489-e13ff637-07b9-4fe5-a04b-bf3467a3046f.png" width="800" />

## Result
<img src="https://user-images.githubusercontent.com/55504676/149818197-dec1b244-ca39-414c-82ab-b776a27041f8.png" width="500" />

## Reference
- Bluetooth Programming
  - https://forums.mbed.com/t/how-do-you-define-gatt-descriptors-using-gattcharacteristic-attributes-gattattribute-class/9809

- Visualization by using MATLAB and USB Serial Port of STM32
  - https://www.mathworks.com/matlabcentral/answers/421759-how-to-read-com-port-data-continuously-and-plot-the-data-live-in-matlab
  - https://os.mbed.com/handbook/USBSerial






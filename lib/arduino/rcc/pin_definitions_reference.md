# RCC Arduino Pin Definitions

# Servo
Vcc    - 5V     //Red
GND    - GND    //Black or Brown
Signal - D9     //White or Orange

## I2C - MPU6050, VL53L0X
SDA - A4
SCL - A5

## L298N - Motor Controller
IN1 - D5
IN2 - D6
IN3 - D7
IN4 - D8
ENA - D10
ENB - D11

## Motor Encoder Connections

### Right Motor
Vcc  - 5V       //Red
GND  - GND      //Black
ENCA - D2       //Blue
ENCB - D12      //Green

### Left Motor
Vcc  - 5V       //Red
GND  - GND      //Black
ENCA - D3       //Blue
ENCB - D13      //Green

## HC-SR04
TRIG - A3
ECHO - D4

## IR-Reflective sensor (for example)
Vcc - 5V
GND - GND
OUT - A1
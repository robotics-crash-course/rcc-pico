from imu import MPU6050
from vl53l0x import VL53L0X
from time import sleep
from machine import Pin, I2C

i2c = I2C(1, sda=Pin(14), scl=Pin(15), freq=400000)
imu = MPU6050(i2c)
tof = VL53L0X(i2c)

tof.set_measurement_timing_budget(40000)
tof.set_Vcsel_pulse_period(tof.vcsel_period_type[0], 12)
tof.set_Vcsel_pulse_period(tof.vcsel_period_type[1], 8)




while True:
#     ax=round(imu.accel.x,2)
#     ay=round(imu.accel.y,2)
#     az=round(imu.accel.z,2)
#     gx=round(imu.gyro.x)
#     gy=round(imu.gyro.y)
    gz=round(imu.gyro.z)
#     tem=round(imu.temperature,2)
#     print("ax",ax,"\t","ay",ay,"\t","az",az,"\t","gx",gx,"\t","gy",gy,"\t","gz",gz,"\t","Temperature",tem,"        ",end="\r")
    
    print(gz, "rad/s", tof.ping()-50, "mm")
    sleep(0.2)
# M031BSP_I2C_LIS3DH_ANGLE

update @ 2019/06/14

>>> Add LIS3DH sample code for M031

>>> PC1 : SCL , PC0 : SDA , PC4 : SA0 , VDDIO/VDD : connect to Vcc

>>> refer to LIS3DH datasheet , 

- if the SA0 pad is connected to the voltage supply , LSb is ‘1’ (address 0011001b)

- if SA0 pad is connected to ground , LSb value is ‘0’ (address 0011000b)

>>> for this example , PC4 set to 1 , mean address is 0x19 (address 0011001b)

>>> Use timer (10s) to swap 2 log message , 

>>> pure raw x , y , z data , ex

- Accelerometer(mg):     X-axis :   -12,  Y-axis :   -20,   Z-axis :   1032

- Accelerometer(mg):     X-axis :    20,  Y-axis :   -20,   Z-axis :   1008

>>> calculate picth/roll by raw data input , ex

- Angle_Calculate:RollAng(    -1.530),PitchAng(     0.219)

- Angle_Calculate:RollAng(    -1.923),PitchAng(     1.282)



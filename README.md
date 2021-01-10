## Autonomous Guided Vehicle
This repository contains the firmware of our graduation project, an autonomous guided vehicle. 

# Description
The overall target function of the vehicle is to collect and carry packages from previously set locations, and to transfer these packages to a dispatch area.
The path of the vehicle is defined apriori and therefore, the goal is maintain and follow the defined path.  

The firmware consists of two main modules:
- Control and decision-making, implemented on Raspberry Pi 3.
- Low-level signal conditioning, implemented on Tiva C - TM4C129 and ATmega328.

The following figure illustrates the architecture of used microcontrollers and developing boards and their corresponding functions:

<img src="photos/ac.png" width="1200"/>

# Control and decision-making
Robot Operating System (ROS) was used to design control and decision-making module, and several nodes were developed to drive the vehicle accurately on the predefined path. 

The developed ROS nodes are:
<img src="photos/ad.png" width="1200"/>

# Low-level signal conditioning

Tiva C - TM4C129 board was used to interface with all peripherals of the vehicle to follow the predefined line smoothly with constant speed.
The peripherals in the system are:

| Peripheral | Function |
| --- | --- |
| Wheels incremental encoders | Measuring vehicle speed and position |
| Steering system absolute encoders | Measuring current steering angle |
| Line follower sensor modules | Detecting the deviation from the predefined line |
| Load cells | Determining the actual load on the vehicle |
| Ultrasonic sensors | Obstacle detection |
| Rear wheel motors | Drive the vehicle on the predefined path |
| Siren | Used as an indicator |
| Rotating lamp | Used as an indicator |

To interface properlly with all peripherals, several ports on Tiva C board were used. The following table summarizes all peripherals connected to the board and their function:

| Port | Pin | Module | Device | Function |  
| --- | --- | --- | --- | --- |
| A | 0 | UART0 (Rx) | PC | On-board diagnostics (OBD) |
| A | 1 | UART0 (Tx) | PC | On-board diagnostics (OBD) |
| A | 4 | UART3 (Rx) | Raspberry Pi | Communication with raspberry pi |
| A | 5 | UART3 (Tx) | Raspberry Pi | Communication with raspberry pi |
| A | 6 | UART2 (Rx) | Bluetooth | Communication with BT module |
| A | 7 | UART2 (Tx) | Bluetooth | Communication with BT module |
| B | 0 | GPIO | Mode Switch | Manual/Autonomous driving switch |
| B | 2 | GPIO | Lamp | Lamp control |
| B | 3 | GPIO | Siren | Siren control |
| C | 4 | Interrupt | Incremental Encoder 1 | Channel Z Counter |
| C | 5 | Interrupt | Incremental Encoder 1 | Channel A Counter |
| C | 6 | GPIO | Incremental Encoder 1 | Channel B Counter |
| D | 0 | SPI - MISO (RX) | Absolute Encoder | SPI2  Master input slave output |
| D | 1 | SPI - MOSI (TX) | Absolute Encoder | SPI2  Master output slave input |
| D | 2 | SPI - SS | Absolute Encoder | SPI2 Select Slave |
| D | 3 | SPI - CLK | Absolute Encoder | SPI2 Clock |
| D | 4 | ADC 7 | Motor driver 1 current sensor | Reading motor driver 1 current |
| D | 5 | ADC 6 | Motor driver 1 current sensor | Reading motor driver 2 current |
| D | 6 | ADC 5 | Motor driver 1 current sensor | Reading motor driver 3 current |
| D | 7 | ADC 4 | Battery volt | Reading battery voltage |






Clone the repo:
``` bash
git clone https://git.uwaterloo.ca/magdaoud/mpc_sim.git
```

We also need to install the following three packages:

``` bash
sudo apt-get install ros-kinetic-ros-control
```

``` bash
sudo apt-get install ros-kinetic-ros-controllers
```

``` bash
sudo apt-get install ros-kinetic-gazebo-ros-control 
```

Then, build using catkin make:
``` bash
catkin_make
```
# Running the simulation

To run the simulation:

``` bash
roslaunch mpc_sim Launch_Simulation.launch 
```

To change the goal pose (change x, y, & theta values)

``` bash
rostopic pub setGoal mpc_sim/Goal_Ackerman "x: 0.0
y: 5.0
theta: 0.0" 
```

**This work is not open for public use and is limited to WATOnomous Design Team use to participate in AutoDrive Challenge.**
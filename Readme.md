# About this repo
This repository contains a simple example of a character driver module for linux and a sample of an user application to communicate with the driver.

# Using the driver code
## Compiling the driver
To compile the driver module run `make` in the build directory.

## Loading the driver
### Using the script
To load the driver module using the provided script use the following command:
```
sudo sh chardriverRun.sh 1 0
```
To enable tail output set the second parameter to '1'
```
sudo sh chardriverRun.sh 1 1
```

### Without the script
To load the driver module without the use of the script use the following command:
```
sudo nsmod CharDriver.ko
```

## Unloading the driver
### Using the script
To unload the driver module using the provided script use the following command:
```
sudo sh chardriverRun.sh 2 0
```
To enable tail output set the second parameter to '1'
```
sudo sh chardriverRun.sh 2 1
```

### Without the script
To load the driver module without the use of the script use the following command:
```
sudo rmmod CharDriver.ko
```

## Script input options
The list of possible input/parameter combinations for the script are as follows:
```
sudo sh chardriverRun.sh x y
```
* Parameter 1 (x):
   * 0 : dmesg
   * 1 : Load module
   * 2 : Unload module
   * 3 : Load and unload moule
* Parameter 2 (y):
  * 0 : desable tail
  * 1 : enable tail

The above list can be obtained by running the following command:
```
sudo sh chardriverRun.sh
```
# Using the user application code
Switch to the "UserApp" folder to work with the user application.
## Compiling the user application
Run the following command to compile:
```
gcc -o UappCD.o UappCD.c
```

## Running the user application
Run the below command to run the user application and interact with the driver:
```
./UappCD.o
```

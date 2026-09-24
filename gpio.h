#ifndef GPIO_H
#define GPIO_H

#define Maxpins 8

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// GPIO pin structure using bit-fields to save memory
typedef struct {
    unsigned int pin_number : 5; // Pin ID (0 to 31)
    unsigned int pin_mode   : 1; // Mode: 0 = Input, 1 = Output
    unsigned int pin_value  : 1; // State: 0 = Low, 1 = High
    unsigned int pullup     : 1; // Pullup: 0 = Disabled, 1 = Enabled
} GPIO;

// Named constants for pin modes
typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_PWM
} GPIOMode;

// Named constants for digital voltage levels
typedef enum {
    GPIO_STATE_LOW = 0,
    GPIO_STATE_HIGH
} GPIOState;

// Named constants for internal pullup resistor states
typedef enum {
    PULL_DISABLED = 0,
    PULL_ENABLED
} GPIOPullup;

// Function declarations for menu and pin operations
void menu();
void initialize(GPIO pins[]);
void configureGpioPin(GPIO pins[]);
void displayAllPins(GPIO pins[]);
void search_pin(GPIO pins[]);
void setPinValue(GPIO pins[]);
void togglePullup(GPIO pins[]);
void faultDetection(GPIO pins[]);
void changeMode(GPIO pins[]);

#endif
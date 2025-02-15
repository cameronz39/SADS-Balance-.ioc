#ifndef INC_STEPPERCONTROL_H_
#define INC_STEPPERCONTROL_H_

#include "main.h"
#include <stdio.h>
#include <string.h>
// #include <cstring>
// #include <cstdio>

#define GPIO_SET_PIN(port, pin)     ((port)->BSRR = (pin))
#define GPIO_CLEAR_PIN(port, pin)     ((port)->BSRR = (pin << 16u))

// Function prototypes and variables can use C++ linkage now.
#ifdef __cplusplus
extern "C" {
#endif

void stepperControl_init();

#ifdef __cplusplus
}
#endif

typedef struct {
    // GPIO information
    GPIO_TypeDef *stepPort;
    uint16_t      stepPin;
    GPIO_TypeDef *dirPort;
    uint16_t      dirPin;
    uint8_t toggleCount;
    // Motion state
    volatile int currentPos;   // current step count
    volatile int desiredPos;   // target step count
    // Timer for stepping (assume your Timer library supports a context pointer)
    // Timer *stepTimer;
} StepperMotor;

extern StepperMotor motor1;
extern StepperMotor motor2;

#endif

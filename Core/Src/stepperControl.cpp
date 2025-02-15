#include <stepperControl.h>
#include "TimerArrayInc/STM32TimerArray.hpp"
// In this example we use the CPU's frequency (what was set in CubeMX by default).
uint32_t timerInputFrequency = F_CPU;
// tick generation running at 10kHz
uint32_t frequencyDivision = timerInputFrequency/10000;
uint32_t timerCounterBits = 16;
char UART_buffer2[50];
TimerArrayControl control(&htim2, timerInputFrequency, frequencyDivision, timerCounterBits);
StepperMotor motor2, motor1;



// Timer motor2_timer(3, true, motor2_callback);

void doStep();
Timer motor1_timer(3, true, doStep);

static uint8_t toggleState1 = 0;
static uint8_t toggleState2 = 0;
void doStep() {
    if (motor1.currentPos < motor1.desiredPos) {
        // Set direction for forward motion
        motor1.dirPort->BSRR = motor1.dirPin;
        if (toggleState1 == 0) { // rising edge: set STEP high
            motor1.stepPort->BSRR = motor1.stepPin;
            toggleState1 = 1;
        } else { // falling edge: set STEP low and update position
            motor1.stepPort->BSRR = (motor1.stepPin << 16);
            toggleState1 = 0;
            motor1.currentPos++;
        }
    } else if (motor1.currentPos > motor1.desiredPos) {
        // Set direction for reverse motion using DIR clear
        motor1.dirPort->BSRR = (motor1.dirPin << 16);
        if (toggleState1 == 0) { // rising edge: set STEP high
            motor1.stepPort->BSRR = motor1.stepPin;
            toggleState1 = 1;
        } else { // falling edge: set STEP low and update position
            motor1.stepPort->BSRR = (motor1.stepPin << 16);
            toggleState1 = 0;
            motor1.currentPos--;
        }
    }
}


void stepperControl_init(){
    motor2.stepPort = STEP2_GPIO_Port;
	motor2.stepPin  = STEP2_Pin;
	motor2.dirPort  = DIR2_GPIO_Port;
	motor2.dirPin   = DIR2_Pin;
	motor2.currentPos = 0;
	motor2.desiredPos = 0;
	// motor2.stepTimer = &motor2_timer;
	motor2.toggleCount = 0;
	// control.attachTimer(&motor2_timer);

	motor1.stepPort = STEP4_GPIO_Port;
	motor1.stepPin  = STEP4_Pin;
	motor1.dirPort  = DIR4_GPIO_Port;
	motor1.dirPin   = DIR4_Pin;
	motor1.currentPos = 0;
	motor1.desiredPos = 0;
	// motor1.stepTimer = &motor1_timer;
	motor1.toggleCount = 0;

	control.attachTimer(&motor1_timer);
	control.begin();
}


extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if ((GPIO_Pin == LIMIT_SWITCH1_Pin) || (GPIO_Pin == LIMIT_SWITCH2_Pin)) {  // Ensure LIMIT_SWITCH_Pin is defined correctly
		int len = snprintf(UART_buffer2, sizeof(UART_buffer2), "Interuppt triggered\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)UART_buffer2, len, 10000);

		control.detachTimer(&motor1_timer);
		// control.detachTimer(&motor2_timer);

		motor1.currentPos = 0;
		motor2.currentPos = 0;
	} else {
		__NOP();

	}
}







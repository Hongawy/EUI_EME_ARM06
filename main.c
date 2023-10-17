#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/fpu.h"
void Init_main(void);
void task_1(void);
void task_2(void);
//Counter to be used inside the SysTick Handler.
uint32_t counter=0;

//SysTick ISR Handler.
void SysTick_Handler(void)
{
    //increment the counter.
    counter++;
}

/* Create array for the stack of Task 1 */
uint32_t stack_RedBlinky[40];
uint32_t *sp_RedBlinky = &stack_RedBlinky[40];/* The stack pointer is initialized to point one word after the stack because the stack grows down that is from the end of the stack array to its beginning */

/* Do the same for Task 2 */
uint32_t stack_GreenBlinky[40];
uint32_t *sp_GreenBlinky = &stack_GreenBlinky[40]; /* The stack pointer is initialized to point one word after the stack because the stack grows down that is from the end of the stack array to its beginning */

int main(void)
{
    //Disable FPU
    FPUDisable();
    //Initialize the main code.
    Init_main();
    //Fill the stack of the first task.
    sp_RedBlinky--; //Stack points at index 39 (final).
    *(sp_RedBlinky--)= 1<<24; //Value of PSR.
    *(sp_RedBlinky--)= (uint32_t) &task_1; //Value of PC.
    *(sp_RedBlinky--)= 6; //Value of LR.
    *(sp_RedBlinky--)= 5; //Value of R12.
    *(sp_RedBlinky--)= 4; //Value of R3.
    *(sp_RedBlinky--)= 3; //Value of R2.
    *(sp_RedBlinky--)= 2; //Value of R1.
    *(sp_RedBlinky)= 1; //Value of R0.

    //Fill the stack of the second task.
    sp_GreenBlinky--; //Stack points at index 39 (final).
    *(sp_GreenBlinky--)= 1<<24; //Value of PSR.
    *(sp_GreenBlinky--)= (uint32_t) &task_2; //Value of PC.
    *(sp_GreenBlinky--)= 6; //Value of LR.
    *(sp_GreenBlinky--)= 5; //Value of R12.
    *(sp_GreenBlinky--)= 4; //Value of R3.
    *(sp_GreenBlinky--)= 3; //Value of R2.
    *(sp_GreenBlinky--)= 2; //Value of R1.
    *(sp_GreenBlinky)= 1; //Value of R0.


    //Call the first task which contains an infinite loop.
    //    task_1();

    while(1)
    {

    }
}
//
//
//
//
//
//
//
//
//
//
//
//
//
void Init_main(void)
{
    //Initialize the System Clock
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    //Initialize clock for PORTF
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //Initialize PINS 1,2,3 as output pins.
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 | GPIO_PIN_3);
    // Enable global interrupts.
    IntMasterEnable();
    //Set the reload value to 100ms inside the SysTick timer. 100ms = (1,600,000 -1) = 1,599,999 ticks
    SysTickPeriodSet(1599999);
    //Enable the SysTick Interrupt.
    SysTickIntEnable();
    //Add the SysTick Handler to the NVIC.
    SysTickIntRegister(SysTick_Handler);
    //Enable the SysTick timer.
    SysTickEnable();
}

void task_1(void)
{
    for(;;)
    {
        //If the counter is divisible by 5 (500ms have passed).
        if(counter%5==0)
        {
            /*Toggle the Red LED.*/
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1));
        }
    }
}

void task_2(void)
{
    for(;;)
    {
        //If the counter is divisible by 10 (1000ms have passed).
        if(counter%10==0)
        {
            /*Toggle the Green LED.*/
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3));
        }
    }
}

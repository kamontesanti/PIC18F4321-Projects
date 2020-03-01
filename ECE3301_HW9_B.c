#include <pic18f4321.h>
#include <math.h>
#include <xc.h>

void __interrupt(high_priority) HighISR(void);  // ISR method prototype
double Sum(int);            // Sum calculation
double RMS(double);         // RMS calculation

// general configurations for the PIC18F4321:
#pragma config OSC = INTIO2
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOR = OFF

unsigned int convert;       // global variable for the conversion value
unsigned int count =  0;    // global variable "count" to ensure 128 samples
double sumResult = 0.0;     // resulting value of the summation
double result = 0.0;        // final result from RMS

// main program:
void main(void) 
{
    ADCON0 = 0x01;          // select channel AN0 and enable ADC module
    ADCON1 = 0x00;          // select reference voltages 0V - 5V and enable AN0
    ADCON2 = 0x0C;          // left-justified, 2Tad, Fosc/4
    PIE1bits.ADIE = 1;      // enable ADC interrupts
    PIR1bits.ADIF = 0;      // clear ADC interrupt flag
    INTCONbits.PEIE = 1;    // enable peripheral interrupts
    INTCONbits.GIE = 1;     // enable global interrupts
    ADCON0bits.GO = 1;      // start the ADC process
    
    while(count < 128);     // wait for the interrupt process which will branch
                            // to the high priority ISR defined below
    
    // after the while loop condition is broken, we must turn off the ADC:
    ADCON0bits.GO = 0;      // turn off the ADC process
    INTCONbits.PEIE = 0;    // disable interrupts for software security
    INTCONbits.GIE = 0;     // disable interrupts for software security
    PIE1bits.ADIE = 0;      // disable interrupts for software security
    
    result = RMS(sumResult);// calculate the RMS value
}

// high priority interrupt service routine:
void __interrupt(high_priority) HighISR(void)
{
    PIR1bits.ADIF = 0;      // clear the ADC interrupt flag
    convert = ADRESH;       // store the value of ADRESH in "convert"
    count++;                // increment the counter since ADC has occurred
    Sum(convert);           // call the Sum method 
    ADCON0bits.GO = 1;      // start the ADC process once again
}

// double return method to calculate the sampling sum:
double Sum(int value)
{
    // add the sampled values and divide by the sampling amount:
    sumResult += ((pow((double)value,2)) / 128);
    return sumResult;   
}

// double return method to calculate the RMS value:
double RMS(double final)
{
    return (sqrt(final));     // return the RMS value
}

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "LCD.h"

int val;
long R;
double Thermistor;

void ADC_Init(void)
{
    DDRC=0x00;      
    ADCSRA = 0x87;  
}

int adc(void)
{
    ADMUX = 0x40;                   
    ADCSRA |= (1<<ADSC);            
    while ((ADCSRA &(1<<ADIF))==0); 
    ADCSRA |=(1<<ADIF);             
    return(ADCW);                   
}

double getTemp(void)
{
    val = adc();                        
    R=((10230000/val) - 10000);         
    Thermistor = log(R);                
    Thermistor = 1 / (0.001129148 + (0.000234125 * Thermistor) + (0.0000000876741 * Thermistor * Thermistor * Thermistor));
    Thermistor = Thermistor - 273.15;   
    
    return Thermistor;
}

int main(void)
{
    char array[20],ohm=0xF4;
    double temp;
    LCD_Init();         
    ADC_Init();         
    LCD_Clear();        
    LCD_String_xy(0, 0,"Temp: ");
    LCD_String_xy(1, 0, "R: ");
    while(1)
    {
        temp = getTemp();   
        memset(array,0,20);
        dtostrf(temp,3,2, array);
        LCD_String_xy(0, 7, array);
        LCD_Char(0xDF);     
        LCD_String("C   ");
        memset(array,0,20);
        sprintf(array,"%ld %c   ",R,ohm);
        LCD_String_xy(1, 3, array);
        _delay_ms(1000);    
    }
}

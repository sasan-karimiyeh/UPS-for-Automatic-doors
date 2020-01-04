#include <tiny13.h>
#include <delay.h>

///////////////////////////
int e,a,j,b;
///////////////////////////
unsigned int adc (void){ //DC Link voltage measurement
ADMUX=0x03;
delay_us(10);
ADCSRA|=0x40; 
while ((ADCSRA & 0x10)==0); 
ADCSRA|=0x10; 
return ADCW; 
} 
/////////////////////////////
void main (void){
DDRB=0x13;
PORTB.2=0;
ADMUX=0x00;
ADCSRA=0x86; 
TIMSK0=0x0a;
TCCR0A=0x03;
TCCR0B=0x05;
#asm
sei
#endasm 

}

 interrupt [TIM0_OVF] void timer2 (void){
 PORTB.4=1;
 TCNT0=164;
 OCR0B=220;   
  e=adc();
  if(e>500){
  a=30550/e;
  OCR0B=157+a;
  goto loop1;
  }
   
  if(e>400){
  a=30350/e;
  OCR0B=157+a;
  }                                           
  
  if(e<400){
  a=30100/e;
  OCR0B=157+a;
  }
  
  if(e<303){                                  
  OCR0B=254;
  }
  loop1:
  
 }
 
  interrupt [TIM0_COMPB] void timer3 (void){ 
  PORTB.4=0;
  }    
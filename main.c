#include <mega16.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#include <math.h>
#include <Receiver_ev1527.h>
////////////////////////////////////////
#define lcd_con   PORTB.7 //LCD back light Control
#define ups       PORTC.2 //ups relays command
#define inv_con   PORTD.6 //dc-dc boost stage start command
#define up        PORTD.3 //up/down direction command
#define com       PORTC.3 //door's com port's relay command 
#define bat_chr   PORTD.5 //battrey charger command
#define out_cur   0x00 //output current measurement port
#define input_vol 0x01 //utility voltage measurement port
#define bat_vol   0x02 //battery voltage measurement port
#define c1        PIND.1 //keypad's the first column
#define c2        PINC.7 //keypad's the second column
#define c3        PINA.3 //keypad's the third column
#define c4        PINA.4 //keypad's the forth column
#define rec       PIND.2 //RF Receiver input port
#define bzr       PORTD.7 //buzzer port
///////////////////////////////////
bit q,d,learning,rev,g,h,t,x,z,p6,p7,p8,p9,p11,memdel;
/*
memdel ==> memory delete indicator
rev ==> reverse mode indicator
learning ==> learning mode indicator
*/
int i,j,a,b,c,s,e,u,y,address[24],cr,vol,vol1,cr1,ab,l;
long int po;
int o2,o3,p2,p3,p4,p5,p12,p13;
int f,add2,add1,data,con,k,o,p,w;
char buf[25];
int bufer[15];
int ivol[8];
eeprom int adress[200]; //user's address 
eeprom int m,n,r;
///////////////////////////////////
unsigned int vol_bat (void){ //battrey voltage measuring 
ADMUX=bat_vol;
delay_us(10);
ADCSRA|=0x40; 
while ((ADCSRA & 0x10)==0); 
ADCSRA|=0x10; 
return ADCW; 
}
///////////////////////////////////
unsigned int cur_out (void){ //output current measuring
ADMUX=out_cur;
delay_us(10);
ADCSRA|=0x40; 
while ((ADCSRA & 0x10)==0); 
ADCSRA|=0x10; 
return ADCW; 
}
///////////////////////////////////
unsigned int vol_in (void){ //utility voltage measurinig
ADMUX=input_vol;
delay_us(10);
ADCSRA|=0x40; 
while ((ADCSRA & 0x10)==0); 
ADCSRA|=0x10; 
return ADCW; 
}
///////////////////////////////////
void buzzer (void){ //buzzer control rutine
TCCR2=0x07;
p2=918;
if(vol1==2){p2=200;}
p3=0;
}
///////////////////////////////////
 void battrey_charge(void){ //battrey charger rutine
 e=vol_bat();
  if(e>540 && e<560){     //full charge
  bat_chr=1;
  }

  if(e<473){              //charging
  bat_chr=0;
  }
 }
 
///////////////////////////////////
void up1 (void){ //door's up direction rutine
TCCR2=0x00;
com=1;
up=1;
} 
///////////////////////////////////
void down1 (void){ //door's down direction rutine
TCCR2=0x00;
com=1;
up=0;
} 
///////////////////////////////////
void stop (void){ //door's stop rutine
p6=1;
com=0;
inv_con=0;
lcd_clear();
lcd_putsf("STOP");
delay_ms(1000);
}
///////////////////////////////////
void inverter_on(void){ //dc-dc boost stage start rutine
 if(inv_con==0){
 inv_con=1;
 delay_ms(5000);
 }
} 
///////////////////////////////////
void operation(void){ //selection of door's condition rutine
 for(j=0;j<r;j++){
  if(add1==adress[j]){
   if(add2==adress[j+1]){
   goto loop9;
   }
   j++;
  }
 }
 goto loop10;
 loop9:
 t=0;  
  switch (data){
  case 4: 
   if(rev){
   up1();
   }
   
   if(rev==0){
   down1();
   }
   break;
  case 1: 
   if(rev==0){
   up1();
   }
   
   if(rev==1){
   down1();
   }
   break;
  case 3:
   stop();
   goto loop10;
   break;
  default:
  break;
  } 
  
  if(vol1==2){
  lcd_con=1;
  o2=0;
  }
 loop10:    
}
///////////////////////////////////
void learning(void){ //user's address saving rutine
/*
at first, new address determine
if the address is new ==> save
if the address is not new ==> quit the routine(loop1)
*/
 for(j=0;j<r;j++){ //new address determining
  if(add1==adress[j]){
   if(add2==adress[j+1]){
   goto loop1;
   }
   j++;
  }
 }
 
 for(j=0;j<200;j++){ //finding empity memory address
  if(0==adress[j]){
   if(0==adress[j+1]){
   r=j;goto loop9;
   }
  j++;
  }
 } 
 loop9:
 adress[r]=add1; //saving
 adress[r+1]=add2; //saving
 sprintf(buf,"%d %d %d",add1,add2,r);
 lcd_clear();
 lcd_puts(buf);
 delay_ms(4000);
 r=r+2; 
 loop1:
}
///////////////////////////////////
void learning_mode(void){ //learning mode rutine
p6=1;
lcd_con=1;
lcd_clear();
lcd_putsf("Learning Mode");
delay_ms(500);
lcd_clear();
lcd_putsf("Please Press a  Key of Remote...");
learning=1;
o=k;
p=m;
l=0;
while(!(l==1 || (((m-p)*32000)+k-o>=3)));
 if(l==1){
 lcd_clear();
 lcd_putsf("Remote Learned");
 lerning();
 delay_ms(3000);
 lcd_clear();
 goto loop11;
 }
 
 if(l==0){
 lcd_clear();
 lcd_putsf("Remote          dont Learned"); 
 delay_ms(1500);
 lcd_clear();
 }
 loop11:
 learning=0;
 h=0;
 p9=0; 
 t=0;
 g=0;
}
///////////////////////////////////
void reverse(void){ //reverse rutine
lcd_con=1;
g=0;
h=0;
t=0;
p9=0;
p6=1;
lcd_clear();
lcd_putsf("UP&Down");
lcd_gotoxy(0,1);
lcd_putsf("Reversed");
rev=~rev;
delay_ms(2000);
}
///////////////////////////////////
void memory_delet(int a){ //memory deleting rutine
memdel=1;
p6=1;
lcd_con=1;
lcd_clear();
lcd_putsf("Memory Delete   Mode");
delay_ms(1500);
 switch (a){
 case 1:
 lcd_clear();
 lcd_putsf("Please Enter a  key of Remoute");
 delay_ms(300);
 o=k;
 p=m;
 while(!(l==2 || (((m-p)*32000)+k-o>=3)));
  if(l==2){ //memory address finding
   for(j=0;j<r;j++){
    if(add1==adress[j]){
     if(add2==adress[j+1]){
     goto loop1;
     }
    j++;
    }
   }
  loop1:
  adress[j]=0;
  adress[j+1]=0;
  sprintf(buf,"%d",j);
  lcd_clear();
  lcd_puts(buf);
  delay_ms(2000);
  }
 lcd_clear();
 lcd_putsf("Remoute Deleted");
 delay_ms(1500);
 h=0;t=0;g=0;p9=0;
 break;
 default:
 break;
 }
 memdel=0;
}
///////////////////////////////////
void remote(void){
p6=1;
lcd_con=1;
lcd_clear();
lcd_putsf("Learn:1");
lcd_gotoxy(0,1);
lcd_putsf("Memory Delet:2");
lcd_gotoxy(8,0);
lcd_putsf("Exit:0"); 
loop5:

delay_ms(300);
if(i==1){learning_mode();goto loop6;}
if(i==2){memory_delet(1);goto loop6;}
if(i==0){h=0;t=0;g=0;p9=0;goto loop6;}
goto loop5;
loop6:

}
///////////////////////////////////
void options(){ //option rutine
p2=0;
p3=0;
p6=1;
lcd_con=1;
lcd_clear();
lcd_putsf("Remote:1");    
lcd_gotoxy(0,1);
lcd_putsf("Reverse:2");
lcd_gotoxy(8,0);
lcd_putsf("D&Tset:3");
lcd_gotoxy(10,1);
lcd_putsf("Exit:0");
delay_ms(400);   
loop4:

delay_ms(300);
if(i==1){remote();goto loop3;}
if(i==2){reverse();goto loop3;}
if(i==0){h=0;t=0;g=0;p9=0;goto loop3;}
goto loop4;
loop3:
}
 
///////////////////////////////////
void power_mesu(void){ //output power measuring rutine
cr=cur_out();
 if(cr>10){
 cr=(cr+49.88)*4.08;
 o2=0;
 goto loop1;
 }

 if(o2==80 && vol1==2){
 o2=0;
 inv_con=0;
 lcd_con=0;
 ups=0;
 com=0;
 }
 cr=0;
loop1:
cr1=cr/1000;
cr=cr-(1000*cr1);
cr=cr/100;
sprintf(buf," %d.%d A",cr1,cr);
lcd_gotoxy(10,0);
lcd_puts(buf);
}     
///////////////////////////////////
void main (void){
DDRB=0xff;
DDRA=0x20;
PORTA=0x18;
DDRD=0xf8;
DDRC=0x0d;
PORTC.7=1;
PORTD.1=1;
lcd_con=1;
ADCSRA=0x86; //ADC config
MCUCR=0x01; //external interrupts
MCUCSR=0;
GIFR=0x40;
GICR=0x40;
TIMSK=0x51; //timer config
TCCR0=0x03;     //normal
TCCR1A=0x00;
TCCR1B=0x0d;    //ctc
OCR1A=31250;
lcd_init(16); //lcd config
lcd_clear();
lcd_putsf("Safe Gate Co");
lcd_gotoxy(0,1);
lcd_putsf("www.SGCO.co.ir");
delay_ms(2000);
lcd_clear();
lcd_putsf("SG1502");
lcd_gotoxy(0,1);
lcd_putsf("UPS 500w");
delay_ms(2000); 
#asm("sei")
if(r==-1){r=0;}     
 while(1){ 
 if(c1==0){reverse();}
 a=vol_in();
 e=vol_bat();              
 p4=521;
 if(bat_chr==0){p4-=80;}
 if(a>p4 && e<561){vol1=1;}       //normal
 if(a<p4 && e<561){vol1=2;}       //inverter
 if(e>561){vol1=3;}               //no battrey
  if(ab!=vol1){p6=1;}
  ab=vol1;
  switch(vol1){
  case 1:
  ups=1;
  battrey_charge();
   if(p6){
   lcd_gotoxy(0,0);
   lcd_putsf("NORMAL    ");
   TCCR2=0;
   p6=0;
   }
  break;
  case 2:
   if(p6){
   ups=0;
   lcd_gotoxy(0,0);
   lcd_putsf("INVERTER  ");
   delay_ms(1000);
   lcd_con=0;
   buzzer();
   p6=0;
   }
  break;
  case 3:
   if(p6){
   ups=1;
   bat_chr=0;
   lcd_gotoxy(0,0);
   lcd_putsf("No BATTREY");  
   buzzer();
   p6=0;
   }
  break;
  default:
  break; 
  }
  
  while(c2==0){
  if(vol1==2){inverter_on();}
  com=1;
  up=1;
  x=1; 
  power_mesu();
  }
  
  while(c3==0){
  if(vol1==2){inverter_on();}
  com=1;
  up=0;
  x=1;
  power_mesu();
  }
  if(c4==0){learning_mode();}
  if(x==1){com=0;x=0;}
  power_mesu();
 o2++;
 if(o2==32000){o2=0;}              
 if(m>32000){m=0;n++;}                 //m ful==>88days
  delay_ms(100); 
 }
}  
/////////////////////////////////////////////////////////
 interrupt[TIM0_OVF] void counter (void){
 f++; 
 }
/////////////////////////////////////////////////////////
 interrupt[EXT_INT0] void reciever (void){ //RF input  interrupt
 receiver_ev1527(f,s,&add1,&add2,&data,&con,&w);
 f=0;
 TCNT0=0;
  if(con==2){
  s=w/3;
  if(learning){l=1;}
  if(learning==0 && memdel==0){operation();}
  if(learning==0 && memdel==1){l=2;}
  delay_ms(100);
  }
 }
///////////////////////////////////////////////////////// 
 interrupt[TIM1_COMPA] void timer (void){//ful==>4sec
 k++;
 if(k==32000){k=0;m++;}        //k ful==>35 hours
 }
/////////////////////////////////////////////////////////
 interrupt[TIM2_OVF] void bzr1 (void){ //timer for buzzer
 p3++;
 if(p3==p2){bzr=1;}
  if(p3==3+p2){
  bzr=0;
  p3=0;    
  e=vol_bat();
   if(e<561){
   if(e<450){goto loop1;}
   p2=(e*5.5675)-2454;
   loop1:
   }
  }
 }  
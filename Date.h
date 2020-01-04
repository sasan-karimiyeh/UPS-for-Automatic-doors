void mil2shm (unsigned char day,unsigned char mon,unsigned char year,unsigned char *rooz,unsigned char *mah,unsigned char *sal){
int a,i;
char buf[25];
bit b=0;
char month[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
if(year%4==0){month[1]=29;}
if(year%4==1){b++;}
a=0;
 for(i=0;i<mon-1;i++){          //calculate day of year
 a+=month[i];
 }
 a+=day;       
 if(a<=79){                     //iranian old year
 a=a+10+b;
  if(a%30==0){
  *mah=9+(a/30);
  *sal=year+78;
  *rooz=30;
  }
  
  if(a%30!=0){
  *rooz=a%30;
  *mah=10+(a/30);
  *sal=year+78;
  }
  goto loop1; 
 }
 
 if(a>79){        //Iranian new year
 a-=79;
  if(a<=186){     //in first half of Iranian new year
   if(a%31==0){
   *mah=a/31;
   *sal=year+79;
   *rooz=31;
   goto loop1;   
   }

   if(a%31!=31){
   *rooz=a%31;
   *mah=1+(a/31);
   *sal=year+79;
   } 
  }
  
  if(a>186){     //in second half of Iranian new year
  a-=186;
   if(a%30==0){
   *mah=6+(a/30);
   *sal=year+79;
   *rooz=30;
   }

   if(a%30!=30){
   *rooz=a%30;
   *mah=7+(a/30);
   *sal=year+79;
   }
  }
 } 
 loop1:
}

void shm2mil (unsigned char rooz,unsigned char mah,unsigned char sal,unsigned char *day,unsigned char *mon,unsigned char *year){
int a,i;
bit b,c;
char buf[25];
int month[9]={ 30, 31, 30, 31, 31, 30, 31, 30, 31};
b=0;
if(sal%4==3){b++;}             //leap year in geargian
 a=0;
 for(i=0;i<mah-1;i++){
 a=a+(31-(i/6));
 }
 a+=rooz;                       //calculate day of year 
 if(a<=(286+b)){               //in old year of geargian
 *year=sal-79;
 if(*year%4==0){c++;}
 if(a<(12+c)){*mon=3;*day=a+20-c;}
  if(a>=(12+c)){
  a-=(11+c);
  i=0;
   for(i=0;a>30;i++){
   a-=month[i];
   if(a==0){a+=month[i];goto loop1;}
   }
   loop1:
   *mon=4+i;
   *day=a;
  }
 } 
 
 if(a>(286+b)){                //in new year of geargian
 *year=sal-78;
 a-=(286+b);
 if(*year%4==0){c++;}
 if(a>(59+c)){*mon=3;*day=a-(59+c);}    
 if(a<32){*mon=1;*day=a;}
 if(31<a && a<(58+c)){*mon=2;*day=a-31;} 
 }
}

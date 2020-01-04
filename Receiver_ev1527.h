int address[24];
int bq,dq;
void receiver_ev1527 (int c,int h,int *add1,int *add2,int *data,int *con,int *b){
#define rec PIND.2      //con=0:not rec 1:continue 2:finish
#define thr 250         //threshold 250ms
int a,d,e,ll,lh,hl,hh;
int address1[2];
char buf[25];
a=c*2048+(8*TCNT0);
hh=*b+thr;
hl=*b-thr;
lh=h+thr+100;
ll=h-thr; 
 if(bq==0 && a<15000 && a>7000 && rec){  //premable 
 *b=a*0.0967741935483871;
 bq++;
 for(d=0;d<24;d++){address[d]=0;}
 goto loop1;
 }
  
  if(bq==1 && a>hl && a<hh && rec==0){
  bq++;
  goto loop1;
  } 
 
  if(bq==2 && a<650 && a>ll  && rec){ //high
  bq--;
  address[dq]=1;
  dq++;
  if(dq==24){goto loop2;}
  goto loop1;
  }
  
  if(bq==1 && a<lh && a>ll&& rec==0){
  bq++;
  goto loop1;
  }
  
  if(bq==2 && a<hh && a>hl &&rec){ //low
  bq--;
  address[dq]=0;
  dq++;
  if(dq==24){goto loop2;}
  goto loop1;
  }
  
  *con=0;
  bq=0;
  dq=0;
  goto loop4;
  loop2:
  for(d=0;d<2;d++){
  address1[d]=0;
  }
     
  for(d=0;d<2;d++){         //integer combine
   for(e=d*10;e<10+(d*10);e++){
   address1[d]=address1[d]+(address[e]*pow(2,e-(d*10)));
   }
  }
  *add1=address1[0];
  *add2=address1[1];
  for(d=0;d<4;d++){
  if(address[d+20]==1){*data=d+1;goto loop3;}
  }
  loop3:
  *con=2;
  bq=0;
  dq=0;
  goto loop4;                                          
  loop1:
 *con=1;
 loop4:
}
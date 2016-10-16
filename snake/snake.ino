

#include "HCuOLED.h"
#include "SPI.h"
/* Example bitmap */


#define CS_DI 10

#define DC_DI 9

#define RST_DI 8

#define X1 0
#define Y1 1
#define X2 2
#define Y2 3

struct BlockSnacke
{
  int X;
  int Y; 
};
BlockSnacke _BlockSnacke[101];
int queue[51];
int lengthShake=5;
int ySquare,xSquare;
int speedSnake;
bool firstsquare=true;
bool start=true;
bool isEmptySpace=true;
unsigned long time;
const int up_button = 2;
const int down_button = 4;
const int left_button = 5;
const int right_button = 3;
const int buttons[] = {up_button, down_button, left_button, right_button};
bool bup_button=false,bdown_button=false,bleft_button=false,bright_button=true;
HCuOLED HCuOLED(SH1106, CS_DI, DC_DI, RST_DI); // For SH1106 displays (HCMODU0058 & HCMODU0059)


void setup() 
{
 
    Serial.begin(9600);
    refreshSnake();
    for (int i; i < 4; i++)
    {
   pinMode(buttons[i], INPUT);
   digitalWrite(buttons[i], HIGH);
  }
  
   HCuOLED.Reset();
   
}

void loop() 
{


  HCuOLED.DrawMode(INVERT);
  while(1)
  {
        
     if(start){
       speedSnake=100;
      delay(speedSnake);
      isNewGame();
       HCuOLED.Refresh();
       isNewGame();
       while((digitalRead(left_button)==1) && (digitalRead(up_button)==1) && (digitalRead(down_button)==1) && (digitalRead(right_button)==1))
      delay(50); 
       start=false;

      }
      else{
        if (lengthShake<101){
          //speedShake();
          if(userMove()){
    if (firstsquare) {
        takeSquare();
        firstsquare=false;
      }

      drawSquare();
      drawSnake();
      
    HCuOLED.Refresh();

     drawSquare();
     drawSnake();
          }else{
            isGameOver();
           HCuOLED.Refresh();
            isGameOver();
            while((digitalRead(left_button)==1) && (digitalRead(up_button)==1) && (digitalRead(down_button)==1) && (digitalRead(right_button)==1))
      delay(50); 
            start=true;
              refreshSnake();
            }
          
        }
        else{
       
          isYouWin();
           HCuOLED.Refresh();
            isYouWin();
            while((digitalRead(left_button)==1) && (digitalRead(up_button)==1) && (digitalRead(down_button)==1) && (digitalRead(right_button)==1))
      delay(50); 
            start=true;
              refreshSnake();
              //Serial.println(freeRam());
              //Serial.print("\n");
          }
      } 
  }
}




bool userMove(void){
  isEmptySpace=true;   
     
 if((digitalRead(down_button)==0) && (bdown_button==false)&& (bup_button==false) && (isEmptySpace)) {
  if (isDownEmpty()){
     if(isDownSquare()){
     createNewSnake(2);
     takeSquare();
     }
    else {
    changeMove(2);
    moveSnake();
    }
    bdown_button=true;
    bup_button=false;
    bleft_button=false;
    bright_button=false;
     
    }else isEmptySpace=false;
 }
 if (bdown_button && (isEmptySpace)){
   if (isDownEmpty()){
   if(isDownSquare()){
     createNewSnake(2);
     takeSquare();
     }
  else {
    delay(speedSnake);
  changeMove(2);
  moveSnake();
  }
  }else isEmptySpace=false;
  }
  
   if((digitalRead(right_button)==0) && (bright_button==false)&& (bleft_button==false) && (isEmptySpace)) {
      if(isRightEmpty){
     if(isRightSquare()) {
     createNewSnake(3);
     takeSquare();
     }
    else {
    changeMove(3);
    moveSnake();
    }
    bdown_button=false;
    bup_button=false;
    bleft_button=false;
    bright_button=true;
      }else isEmptySpace=false;
    }
 if (bright_button && (isEmptySpace)){
  if(isRightEmpty()){
   if(isRightSquare()) {
    
     createNewSnake(3);
     takeSquare();
    
     }  
  else{
  changeMove(3); 
  moveSnake();
  delay(speedSnake);
  }
  } else isEmptySpace=false;;
 }
if((digitalRead(up_button)==0) && (bup_button==false)&& (bdown_button==false) && (isEmptySpace)) {
  if (isUpEmpty()){
     if(isUpSquare()){
     createNewSnake(5);
     takeSquare();
     }
    else {
    changeMove(5);
    moveSnake();
    }
    bdown_button=false;
    bup_button=true;
    bleft_button=false;
    bright_button=false;
     
    }else isEmptySpace=false;
 }
 if (bup_button && (isEmptySpace)){
   if (isUpEmpty()){
   if(isUpSquare()){
     createNewSnake(5);
     takeSquare();
     }
  else {
    delay(speedSnake);
  changeMove(5);
  moveSnake();
  }
  }else isEmptySpace=false;
  }
  
    
    if((digitalRead(left_button)==0) && (bleft_button==false) && (bright_button==false) && (isEmptySpace)) {
  if (isLeftEmpty()){
     if(isLeftSquare()){
     createNewSnake(1);
     takeSquare();
     }
    else {
    changeMove(1);
    moveSnake();
    }
    bdown_button=false;
    bup_button=false;
    bleft_button=true;
    bright_button=false;
     
    }else isEmptySpace=false;
 }
 if (bleft_button && (isEmptySpace)){
   if (isLeftEmpty()){
   if(isLeftSquare()){
     createNewSnake(1);
     takeSquare();
     }
  else {
    delay(speedSnake);
  changeMove(1);
  moveSnake();
  }
  }else isEmptySpace=false;
  }
  
  return isEmptySpace;
  }




  
void moveSnake(void){

  for (int i=0;i<lengthShake;i++){
    switch (queue[i])
      {
         case 1:
         {
             if(_BlockSnacke[i].Y-3<3)  _BlockSnacke[i].Y=126;
            _BlockSnacke[i].Y=_BlockSnacke[i].Y-3;
            
            }
            break;
          case 2:
          {
          if(_BlockSnacke[i].X+3>62)  _BlockSnacke[i].X=0;
            _BlockSnacke[i].X=_BlockSnacke[i].X+3;
          }
            break;
          case 3:
          {
              if(_BlockSnacke[i].Y+3>123)  _BlockSnacke[i].Y=0;
            _BlockSnacke[i].Y=_BlockSnacke[i].Y+3;
          }
            break;
          case 5:
          {
            if(_BlockSnacke[i].X-3<3)  _BlockSnacke[i].X=63;
            _BlockSnacke[i].X=_BlockSnacke[i].X-3;
          }
            break;
         default:
         break;
      }
      }
  }

  

void drawSnake(void){
  for (int i=0;i<lengthShake;i++){
    for(int j= _BlockSnacke[i].Y-1;j<_BlockSnacke[i].Y+2;j++)
      for(int k= _BlockSnacke[i].X-1;k<_BlockSnacke[i].X+2;k++)
      HCuOLED.Plot(j,k);
    }
      
  }
void takeSquare(){
  xSquare=1;
  ySquare=1;
   while((xSquare+3)%3!=0){
    delay(10);
        time=millis(); 
        xSquare=time%61+1;
      }
       while((ySquare+3)%3!=0){
        delay(10);
        time=millis(); 
        ySquare=time%125+1;
      }
  }
void drawSquare(){
  for(int j=ySquare-1;j<ySquare+2;j++)
      for(int k= xSquare-1;k<xSquare+2;k++)
       HCuOLED.Plot(j,k);
  }
void createNewSnake(int mov){
  lengthShake++;
  _BlockSnacke[lengthShake-1].X=xSquare;
  _BlockSnacke[lengthShake-1].Y=ySquare;  
  queue[lengthShake-1]=mov;
  speedShake();
  }


  
void changeMove(int mov){
  for (int i=0;i<lengthShake-1;i++){
     queue[i]=queue[i+1];
  }
     queue[lengthShake-1]=mov;
  }



  
bool isRightSquare(){
  if((_BlockSnacke[lengthShake-1].Y+3==ySquare)&&(_BlockSnacke[lengthShake-1].X==xSquare))
  return true; else return false;
  }
bool isLeftSquare(){
  if((_BlockSnacke[lengthShake-1].Y-3==ySquare)&&(_BlockSnacke[lengthShake-1].X==xSquare))
  return true; else return false;
  }
bool isUpSquare(){
  if((_BlockSnacke[lengthShake-1].Y==ySquare)&&(_BlockSnacke[lengthShake-1].X-3==xSquare))
  return true; else return false;
  }
bool isDownSquare(){
  if((_BlockSnacke[lengthShake-1].Y==ySquare)&&(_BlockSnacke[lengthShake-1].X+3==xSquare))
  return true; else return false;
  }

bool isRightEmpty(){
   bool isEmptyThis=true;
  for(int i=0;i<lengthShake-1;i++){
      if((_BlockSnacke[lengthShake-1].Y+3==_BlockSnacke[i].Y)&&(_BlockSnacke[lengthShake-1].X==_BlockSnacke[i].X))
        isEmptyThis=false;
  }
return isEmptyThis;
}

bool isLeftEmpty(){
   bool isEmptyThis=true;
  for(int i=0;i<lengthShake-1;i++)
  {
  if((_BlockSnacke[lengthShake-1].Y-3==_BlockSnacke[i].Y)&&(_BlockSnacke[lengthShake-1].X==_BlockSnacke[i].X))
   isEmptyThis=false;
  }
  return isEmptyThis;
  }
  bool isUpEmpty(){
     bool isEmptyThis=true;
  for(int i=0;i<lengthShake-1;i++){
  if((_BlockSnacke[lengthShake-1].Y==_BlockSnacke[i].Y)&&(_BlockSnacke[lengthShake-1].X-3==_BlockSnacke[i].X))
  isEmptyThis=false;
  }
  return isEmptyThis;
  }
  bool isDownEmpty(){
    bool isEmptyThis=true;
  for(int i=0;i<lengthShake-1;i++){
  if((_BlockSnacke[lengthShake-1].Y==_BlockSnacke[i].Y)&&(_BlockSnacke[lengthShake-1].X+3==_BlockSnacke[i].X))
  isEmptyThis=false;
  }
  return isEmptyThis;
  }

 void isGameOver()
 {
  HCuOLED.Cursor(8,0);
  HCuOLED.SetFont(LCDLarge_24pt);
  HCuOLED.Print("Game Over");

      HCuOLED.Cursor(16,32);
  HCuOLED.SetFont(Terminal_8pt);
  HCuOLED.Print("Your score ");
    HCuOLED.Cursor(48,48);
  HCuOLED.Print(lengthShake);
  } 
 void isNewGame(){
  HCuOLED.Cursor(8,0);
  HCuOLED.SetFont(LCDLarge_24pt);
  HCuOLED.Print("New Game");
   HCuOLED.Cursor(0,32);
  HCuOLED.SetFont(Terminal_8pt);
  HCuOLED.Print("press any key");
  
  }
   void isYouWin(){
  HCuOLED.Cursor(24,8);
  HCuOLED.SetFont(LCDLarge_24pt);
  HCuOLED.Print("You Win");
  }

  void refreshSnake(){
    for (int i=4;i<lengthShake;i++){
    _BlockSnacke[i].X=0;
    _BlockSnacke[i].X=0;
    queue[i]=0;
    }
    lengthShake=5;
  for (int i=0;i<lengthShake;i++){
    _BlockSnacke[i].X=9;
    _BlockSnacke[i].Y=i*3; 
     queue[i]=3;
    }
      
    }

void speedShake(void){
  if (lengthShake<11) speedSnake=100;
  else if (lengthShake<21) speedSnake=50;
    else if (lengthShake<36) speedSnake=25;
    else speedSnake=10;
  }

  int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

/**
 *     
	This file is part of .PNG Arduino Framework.

    .PNG Arduino Framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    .PNG Arduino Framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with .PNG Arduino Framework.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "LedMatrixObject.h"

LedMatrixObject::LedMatrixObject(
			byte LEDARRAY_D, byte LEDARRAY_C, 
			byte LEDARRAY_B, byte LEDARRAY_A, 
			byte LEDARRAY_G, byte LEDARRAY_DI, 
			byte LEDARRAY_CLK, byte LEDARRAY_LAT,
			byte LEDARRAY_D2, byte LEDARRAY_C2, 
      byte LEDARRAY_B2, byte LEDARRAY_A2, 
      byte LEDARRAY_G2, byte LEDARRAY_DI2, 
      byte LEDARRAY_CLK2, byte LEDARRAY_LAT2){

	this->LEDARRAY_D = LEDARRAY_D;
	this->LEDARRAY_C = LEDARRAY_C;
	this->LEDARRAY_B = LEDARRAY_B;
	this->LEDARRAY_A = LEDARRAY_A;
	this->LEDARRAY_G = LEDARRAY_G;
	this->LEDARRAY_DI = LEDARRAY_DI;
	this->LEDARRAY_CLK = LEDARRAY_CLK;
	this->LEDARRAY_LAT = LEDARRAY_LAT;

    this->LEDARRAY_D2 = LEDARRAY_D2;
  this->LEDARRAY_C2 = LEDARRAY_C2;
  this->LEDARRAY_B2 = LEDARRAY_B2;
  this->LEDARRAY_A2 = LEDARRAY_A2;
  this->LEDARRAY_G2 = LEDARRAY_G2;
  this->LEDARRAY_DI2 = LEDARRAY_DI2;
  this->LEDARRAY_CLK2 = LEDARRAY_CLK2;
  this->LEDARRAY_LAT2 = LEDARRAY_LAT2;

	configure();
}


void LedMatrixObject::configure(){
	pinMode(LEDARRAY_D, OUTPUT); 
	pinMode(LEDARRAY_C, OUTPUT);
	pinMode(LEDARRAY_B, OUTPUT);
	pinMode(LEDARRAY_A, OUTPUT);
	pinMode(LEDARRAY_G, OUTPUT);
	pinMode(LEDARRAY_DI, OUTPUT);
	pinMode(LEDARRAY_CLK, OUTPUT);
	pinMode(LEDARRAY_LAT, OUTPUT);

  pinMode(LEDARRAY_D2, OUTPUT); 
  pinMode(LEDARRAY_C2, OUTPUT);
  pinMode(LEDARRAY_B2, OUTPUT);
  pinMode(LEDARRAY_A2, OUTPUT);
  pinMode(LEDARRAY_G2, OUTPUT);
  pinMode(LEDARRAY_DI2, OUTPUT);
  pinMode(LEDARRAY_CLK2, OUTPUT);
  pinMode(LEDARRAY_LAT2, OUTPUT);
}

void LedMatrixObject::clear(){
	for(int i = 0; i < 32; i++)
		Word1[i] = 0;
}

void LedMatrixObject::clearScenes(){
	for(byte i = 0; i < 16; i++)
		for(byte k = 0; k < 16; k++)
			this->Scene[i][k] = 1;
  for(byte i = 0; i < 16; i++)
    for(byte k = 0; k < 16; k++)
      this->Scene2[i][k] = 1;
}

void LedMatrixObject::sceneToWord(){
	//clear word
	clear();

	int i, k, key = 0;
	unsigned int value;
	for(i = 0; i < 16; i++){
		for(k = 0; k < 16; k++){
			if(i < 8){
				value = Scene[i][k] << (7 - i);
				Word1[15 - k] += value;
			} else {
				value = Scene[i][k] << (15 - i);
				Word1[31 - k] += value;
			}
		}
	}
}

void LedMatrixObject::sceneToWord2(){
  //clear word
  clear();

  int i, k, key = 0;
  unsigned int value;
  for(i = 0; i < 16; i++){
    for(k = 0; k < 16; k++){
      if(i < 8){
        value = Scene2[i][k] << (7 - i);
        Word1[15 - k] += value;
      } else {
        value = Scene2[i][k] << (15 - i);
        Word1[31 - k] += value;
      }
    }
  }
}

void LedMatrixObject::draw(){
	sceneToWord();
	display();
  sceneToWord2();
  display2();
	clearScenes();
}

void LedMatrixObject::display(){
	unsigned char i;

	for( i = 0 ; i < 16 ; i++ )
	{
		digitalWrite(LEDARRAY_G, HIGH);		
		
		Display_Buffer[0] = Word1[i];		
		Display_Buffer[1] = Word1[i+16];

		send(Display_Buffer[1]);
		send(Display_Buffer[0]);

		digitalWrite(LEDARRAY_LAT, HIGH);					 
		delayMicroseconds(1);
	
		digitalWrite(LEDARRAY_LAT, LOW);
		delayMicroseconds(1);

		scan_Line(i);

		digitalWrite(LEDARRAY_G, LOW);
		
		delayMicroseconds(100);		
	}	
}

void LedMatrixObject::display2(){
  unsigned char i;

  for( i = 0 ; i < 16 ; i++ )
  {
    digitalWrite(LEDARRAY_G2, HIGH);   
    
    Display_Buffer[0] = Word1[i];   
    Display_Buffer[1] = Word1[i+16];

    send2(Display_Buffer[1]);
    send2(Display_Buffer[0]);

    digitalWrite(LEDARRAY_LAT2, HIGH);          
    delayMicroseconds(1);
  
    digitalWrite(LEDARRAY_LAT2, LOW);
    delayMicroseconds(1);

    scan_Line2(i);

    digitalWrite(LEDARRAY_G2, LOW);
    
    delayMicroseconds(100);   
  } 
}


void LedMatrixObject::send(unsigned int dat) {
	unsigned char i;
	digitalWrite(LEDARRAY_CLK, LOW);
	delayMicroseconds(1);;	
	digitalWrite(LEDARRAY_LAT, LOW);
	delayMicroseconds(1);;

	for( i = 0 ; i < 8 ; i++ )
	{
		if( dat&0x01 )
		{
			digitalWrite(LEDARRAY_DI, HIGH);	
		}
		else
		{
			digitalWrite(LEDARRAY_DI, LOW);
		}

		delayMicroseconds(1);
		digitalWrite(LEDARRAY_CLK, HIGH);				  
			delayMicroseconds(1);
		digitalWrite(LEDARRAY_CLK, LOW);
			delayMicroseconds(1);		
		dat >>= 1;
			
	}			
}

void LedMatrixObject::send2(unsigned int dat) {
  unsigned char i;
  digitalWrite(LEDARRAY_CLK2, LOW);
  delayMicroseconds(1);;  
  digitalWrite(LEDARRAY_LAT2, LOW);
  delayMicroseconds(1);;

  for( i = 0 ; i < 8 ; i++ )
  {
    if( dat&0x01 )
    {
      digitalWrite(LEDARRAY_DI2, HIGH);  
    }
    else
    {
      digitalWrite(LEDARRAY_DI2, LOW);
    }

    delayMicroseconds(1);
    digitalWrite(LEDARRAY_CLK2, HIGH);         
      delayMicroseconds(1);
    digitalWrite(LEDARRAY_CLK2, LOW);
      delayMicroseconds(1);   
    dat >>= 1;
      
  }     
}


void LedMatrixObject::scan_Line(unsigned int m) {	
	switch(m)
	{
		case 0:			
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 					
			break;
		case 1:					
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 2:					
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 3:					
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 4:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 5:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 6:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 7:
			digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 8:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 9:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;	
		case 10:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 11:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 12:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 13:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		case 14:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW); 		
			break;
		case 15:
			digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH); 		
			break;
		default : break;	
	}
}

void LedMatrixObject::scan_Line2(unsigned int m) {  
  switch(m)
  {
    case 0:     
      digitalWrite(LEDARRAY_D2, LOW);digitalWrite(LEDARRAY_C2, LOW);digitalWrite(LEDARRAY_B2, LOW);digitalWrite(LEDARRAY_A2, LOW);          
      break;
    case 1:         
      digitalWrite(LEDARRAY_D2, LOW);digitalWrite(LEDARRAY_C2, LOW);digitalWrite(LEDARRAY_B2, LOW);digitalWrite(LEDARRAY_A2, HIGH);     
      break;
    case 2:         
      digitalWrite(LEDARRAY_D2, LOW);digitalWrite(LEDARRAY_C2, LOW);digitalWrite(LEDARRAY_B2, HIGH);digitalWrite(LEDARRAY_A2, LOW);     
      break;
    case 3:         
      digitalWrite(LEDARRAY_D2, LOW);digitalWrite(LEDARRAY_C2, LOW);digitalWrite(LEDARRAY_B2, HIGH);digitalWrite(LEDARRAY_A2, HIGH);    
      break;
    case 4:
      digitalWrite(LEDARRAY_D2, LOW);digitalWrite(LEDARRAY_C2, HIGH);digitalWrite(LEDARRAY_B2, LOW);digitalWrite(LEDARRAY_A2, LOW);     
      break;
    case 5:
      digitalWrite(LEDARRAY_D2, LOW);digitalWrite(LEDARRAY_C2, HIGH);digitalWrite(LEDARRAY_B2, LOW);digitalWrite(LEDARRAY_A2, HIGH);    
      break;
    case 6:
      digitalWrite(LEDARRAY_D2, LOW);digitalWrite(LEDARRAY_C2, HIGH);digitalWrite(LEDARRAY_B2, HIGH);digitalWrite(LEDARRAY_A2, LOW);    
      break;
    case 7:
      digitalWrite(LEDARRAY_D2, LOW);digitalWrite(LEDARRAY_C2, HIGH);digitalWrite(LEDARRAY_B2, HIGH);digitalWrite(LEDARRAY_A2, HIGH);     
      break;
    case 8:
      digitalWrite(LEDARRAY_D2, HIGH);digitalWrite(LEDARRAY_C2, LOW);digitalWrite(LEDARRAY_B2, LOW);digitalWrite(LEDARRAY_A2, LOW);     
      break;
    case 9:
      digitalWrite(LEDARRAY_D2, HIGH);digitalWrite(LEDARRAY_C2, LOW);digitalWrite(LEDARRAY_B2, LOW);digitalWrite(LEDARRAY_A2, HIGH);    
      break;  
    case 10:
      digitalWrite(LEDARRAY_D2, HIGH);digitalWrite(LEDARRAY_C2, LOW);digitalWrite(LEDARRAY_B2, HIGH);digitalWrite(LEDARRAY_A2, LOW);    
      break;
    case 11:
      digitalWrite(LEDARRAY_D2, HIGH);digitalWrite(LEDARRAY_C2, LOW);digitalWrite(LEDARRAY_B2, HIGH);digitalWrite(LEDARRAY_A2, HIGH);     
      break;
    case 12:
      digitalWrite(LEDARRAY_D2, HIGH);digitalWrite(LEDARRAY_C2, HIGH);digitalWrite(LEDARRAY_B2, LOW);digitalWrite(LEDARRAY_A2, LOW);    
      break;
    case 13:
      digitalWrite(LEDARRAY_D2, HIGH);digitalWrite(LEDARRAY_C2, HIGH);digitalWrite(LEDARRAY_B2, LOW);digitalWrite(LEDARRAY_A2, HIGH);     
      break;
    case 14:
      digitalWrite(LEDARRAY_D2, HIGH);digitalWrite(LEDARRAY_C2, HIGH);digitalWrite(LEDARRAY_B2, HIGH);digitalWrite(LEDARRAY_A2, LOW);     
      break;
    case 15:
      digitalWrite(LEDARRAY_D2, HIGH);digitalWrite(LEDARRAY_C2, HIGH);digitalWrite(LEDARRAY_B2, HIGH);digitalWrite(LEDARRAY_A2, HIGH);    
      break;
    default : break;  
  }
}

void LedMatrixObject::setScene(unsigned char  Scene[16][16]){
	for(byte i = 0; i < 16; i++)
		for(byte k = 0; k < 16; k++)
			this->Scene[i][k] = Scene[i][k];
}
void LedMatrixObject::setScene2(unsigned char  Scene[16][16]){
  for(byte i = 0; i < 16; i++)
    for(byte k = 0; k < 16; k++)
      this->Scene2[i][k] = Scene[i][k];
}

void LedMatrixObject::setLedOn(int x, int y){
	Scene[x][y] = 0;
}

void LedMatrixObject::setLedOff(int x, int y){
	Scene[x][y] = 1;
}

void LedMatrixObject::toggleLed(int x, int y){
	Scene[x][y] = !Scene[x][y];
}
void LedMatrixObject::setLedOn2(int x, int y){
  Scene2[x][y] = 0;
}

void LedMatrixObject::setLedOff2(int x, int y){
  Scene2[x][y] = 1;
}

void LedMatrixObject::toggleLed2(int x, int y){
  Scene2[x][y] = !Scene[x][y];
}

void LedMatrixObject::displayPauseScene(){
 unsigned char  img[16][16] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
      //this->clearScenes();
      this->setScene(img);
      this->draw();
}
void LedMatrixObject::displayHomeScene(){
unsigned char  img[16][16] = {
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
 //this->clearScenes();
      this->setScene(img);
      this->draw();
}

void LedMatrixObject::displayDoneScene(){
unsigned char  img[16][16] = {
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
   {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
   {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
   {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
   {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
     {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
     //this->clearScenes();
      this->setScene(img);
      this->draw();
  }


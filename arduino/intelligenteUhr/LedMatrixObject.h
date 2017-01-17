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


#include "stdlib.h"
#include "Arduino.h"

#ifndef LED_MATRIX_OBJECT_H
#define LED_MATRIX_OBJECT_H
typedef void (*CallBackType)();


class LedMatrixObject{
private:
	byte LEDARRAY_D;
	byte LEDARRAY_C;
	byte LEDARRAY_B;
	byte LEDARRAY_A;
	byte LEDARRAY_G;
	byte LEDARRAY_DI;
	byte LEDARRAY_CLK;
	byte LEDARRAY_LAT;
  
  byte LEDARRAY_D2;
  byte LEDARRAY_C2;
  byte LEDARRAY_B2;
  byte LEDARRAY_A2;
  byte LEDARRAY_G2;
  byte LEDARRAY_DI2;
  byte LEDARRAY_CLK2;
  byte LEDARRAY_LAT2;

	unsigned char  Scene[16][16];
  unsigned char  Scene2[16][16];
	unsigned char Display_Buffer[2];
	unsigned int Word1[32];

	void configure();
	void sceneToWord();
  void sceneToWord2();
	void display();
  void display2();
	void send(unsigned int dat);
  void send2(unsigned int dat);
	void scan_Line(unsigned int m);
 void scan_Line2(unsigned int m);
	void clearScenes();


public:
	LedMatrixObject(byte LEDARRAY_D, byte LEDARRAY_C, byte LEDARRAY_B, byte LEDARRAY_A, byte LEDARRAY_G, byte LEDARRAY_DI, byte LEDARRAY_CLK, byte LEDARRAY_LAT,byte LEDARRAY_D2, byte LEDARRAY_C2, byte LEDARRAY_B2, byte LEDARRAY_A2, byte LEDARRAY_G2, byte LEDARRAY_DI2, byte LEDARRAY_CLK2, byte LEDARRAY_LAT2);
	void clear();
	void draw();
	void setScene(unsigned char  Scene[16][16]);
  void setScene2(unsigned char  Scene[16][16]);
	void setLedOn(int x, int y);
	void setLedOff(int x, int y);
	void toggleLed(int x, int y);
  void setLedOn2(int x, int y);
  void setLedOff2(int x, int y);
  void toggleLed2(int x, int y);
 void displayPauseScene();
 void displayHomeScene();
 void displayDoneScene();

};


#endif // LED_MATRIX_OBJECT_H

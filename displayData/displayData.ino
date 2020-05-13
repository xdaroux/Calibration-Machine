/*
 * TM1637.cpp
 * A library for the 4 digit display
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Frankie.Chu
 * Create Time: 9 April,2012
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "TM1637.h"
#define CLK 4//pins definitions for TM1637 and can be changed to other ports
#define DIO 5
TM1637 tm1637(CLK,DIO);
TM1637 rpm(43,42);
TM1637 acc0(53,52);
TM1637 acc1(51,50);
TM1637 acc2(49,48);
TM1637 acc3(47,46);
void setup()
{
  tm1637.init();
  acc0.init();
  acc1.init();
  acc2.init();
  acc3.init();
  rpm.init();
  
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  acc0.set(BRIGHT_TYPICAL);
  acc1.set(BRIGHT_TYPICAL);
  acc2.set(BRIGHT_TYPICAL);
  acc3.set(BRIGHT_TYPICAL);
  rpm.set(BRIGHT_TYPICAL);
  acc0.point(POINT_ON);
  Serial.begin(9600);
}
void loop()
{
  int8_t NumTab[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//0~9,A,b,C,d,E,F
  int8_t ListDisp[4];
  unsigned char i = 0;
  unsigned char count = 0;
  uint8_t buff[2];
  delay(150);
  while(1)
  {
   /* i = count;
    count ++;
    if(count == sizeof(NumTab)) count = 0;
    for(unsigned char BitSelect = 0;BitSelect < 4;BitSelect ++)
    {
      ListDisp[BitSelect] = NumTab[i];
      i ++;
      if(i == sizeof(NumTab)) i = 0;
    }
    acc0.display(0,0); // (Where,Value)
    acc0.display(1,2);
    acc0.display(2,3);
    acc0.display(3,4);

    acc1.display(0,0); // (Where,Value)
    acc1.display(1,2);
    acc1.display(2,3);
    acc1.display(3,4);

    acc2.display(0,0); // (Where,Value)
    acc2.display(1,2);
    acc2.display(2,3);
    acc2.display(3,4);

    acc3.display(0,0); // (Where,Value)
    acc3.display(1,2);
    acc3.display(2,3);
    acc3.display(3,4);

    rpm.display(0,0); // (Where,Value)
    rpm.display(1,2);
    rpm.display(2,3);
    rpm.display(3,4);*/
    uptade_display_acc(acc0,16,25);
    delay(300);
    split_2_digit_number(56, buff);
    Serial.print("Potition 0 : ");Serial.println(buff[0]); // suppose to be 5 
     Serial.print("Potition 1 : ");Serial.println(buff[1]); // suppose to be 6 
     
    
    
  }
}

void uptade_display_acc(TM1637 acc,uint8_t angle, uint8_t poids)
{
  uint8_t buff[2];
    //Angle
    split_2_digit_number(angle,buff);
    acc.display(0,buff[0]); // (Where,Value)
    acc.display(1,buff[1]);
    //poids
    split_2_digit_number(poids,buff);
    acc.display(2,buff[0]);
    acc.display(3,buff[1]);
}

void split_2_digit_number(uint8_t num, uint8_t* buff)
{
  buff[0] = num / 10; // dizaine 
  buff[1] = num % 10; // unite
}

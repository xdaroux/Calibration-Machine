
/*
*   Processeur : Arduino Mega or Mega 2560
*   Pin Configuration : 
*
*   display 1 : rpm
*               DIO : 42
*               CLK : 43 
*
*   display 2 : acc0
*               DIO : 52
*               CLK : 53
*
*   display 3 : acc1
*               DIO : 50
*               CLK : 51 
*
*   display 4 : acc2
*               DIO : 48
*               CLK : 49
*
*   display 4 : acc3
*               DIO : 46
*               CLK : 47
*
*/
#pragma once


#include <Arduino.h>
#include "TM1637.h"

#define LUMINOSITE 2 // Echelle de 0 a 7 
#define POINT_ON 1
#define POINT_OFF 0


#define RPM_DIO 42
#define RPM_CLK 43

#define ACC0_DIO 52
#define ACC0_CLK 53

#define ACC1_DIO 50
#define ACC1_CLK 51

#define ACC2_DIO 48
#define ACC2_CLK 49

#define ACC3_DIO 46
#define ACC3_CLK 47



void setup_displayData();





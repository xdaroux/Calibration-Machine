#include "displayData.h"

    TM1637 RpmDisplay(RPM_CLK, RPM_DIO);
    TM1637 Acc0Display(ACC0_CLK, ACC0_DIO);
    TM1637 Acc1Display(ACC1_CLK, ACC1_DIO);
    TM1637 Acc2Display(ACC2_CLK, ACC2_DIO);
    TM1637 Acc3Display(ACC3_CLK, ACC3_DIO);
    
void setup_displayData()
{



    RpmDisplay.init(); 
    Acc0Display.init();
    Acc1Display.init();
    Acc2Display.init();
    Acc3Display.init(); 

    
    RpmDisplay.set(LUMINOSITE); // Echelle de 0 a 7
    Acc0Display.set(LUMINOSITE);
    Acc1Display.set(LUMINOSITE);
    Acc2Display.set(LUMINOSITE);
    Acc3Display.set(LUMINOSITE);

    Acc0Display.point(POINT_ON);
    Acc1Display.point(POINT_ON);
    Acc2Display.point(POINT_ON);
    Acc3Display.point(POINT_ON);
   
}

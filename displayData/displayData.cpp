#include "displayData.h"

void setup_displayData()
{
    rpm.init(); 
    acc0.init();
    acc1.init();
    acc2.init();
    acc3.init(); 

    
    rpm.set(LUMINOSITE); // Echelle de 0 a 7
    acc0.set(LUMINOSITE);
    acc1.set(LUMINOSITE);
    acc2.set(LUMINOSITE);
    acc3.set(LUMINOSITE);

    acc0.point(POINT_ON);
    acc1.point(POINT_ON);
    acc2.point(POINT_ON);
    acc3.point(POINT_ON);
   
}
#pragma once
#include "RPM.h"
#include "ACCELEROMETRE.h"
/*====================================DEFINE====================================*/

//etat
#define INIT 0
#define ARRETER 1
#define STABLE 2  // faut que stable sois =  2, car dans RPM #define STABLE 2
#define START 3
#define TEST 4
#define CALCUL_INTERMEDIAIRE 5
#define CALCUL 6
#define AFFICHER 7
//fin etat

#define NB_TEST 10

//acc
#define DIMENSION 100// sois la dimension du tableau pour le nb de lecture dansune rotation du systeme

  

/*====================================TYPEDEF====================================*/
typedef struct{
  volatile int etat; 
  int angleMoyen; 
  int poidsMoyen; 

  int angleMoyenTest[NB_TEST];
  int poidsMoyenTest[NB_TEST];
  int numerosTest;
  
}T_calibration;


/*===============================GLOBAL Variable====================================*/

T_calibration CALIBRATION;
T_RPM RPM;
T_ACC ACC[DIMENSION];


/*====================================Declaration====================================*/
void Calibration_init(T_calibration * calib);





/*====================================Definition====================================*/
void Calibration_init(T_calibration * calib)
{
  calib->etat = INIT;
  calib->angleMoyen = 0;
  calib->poidsMoyen = 0;
  int i;
  for(i=0;i<NB_TEST;i++)
  {
   calib->angleMoyenTest[i] = 0;
   calib->poidsMoyenTest[i] = 0; 
  }
  calib->numerosTest = 0;
  
}

#pragma once

/*Include======================================================================*/

#include <Arduino.h>
#include "Rpm.h"
#include "Accelerometre.h"

/*Define=======================================================================*/

//etat
enum
{
  INIT,
  ARRETER,
  START,
  TEST,
  CALCUL_INTERMEDIAIRE,
  CALCUL,
  AFFICHER
}State;
//fin etat

#define NB_TEST 10

//acc
#define DIMENSION 100 // sois la dimension du tableau pour le nb de lecture dans une rotation du systeme

/*====================================TYPEDEF====================================*/
typedef struct
{
  volatile int etat;
  int angleMoyen;
  int poidsMoyen;

  int angleMoyenTest[NB_TEST];
  int poidsMoyenTest[NB_TEST];
  int numerosTest;

} T_calibration;
/*===============================GLOBAL Variable====================================*/

T_calibration Calibration;
T_RPM Rpm;
T_ACC ACC[DIMENSION];
T_ACC AccMax;
T_ACC AccMin;
uint16_t RawAcc[DIMENSION];


/*====================================Declaration====================================*/
void Calibration_init(T_calibration *calib);

/*====================================Definition====================================*/
void Calibration_init(T_calibration *calib)
{
  calib->etat = INIT;
  calib->angleMoyen = 0;
  calib->poidsMoyen = 0;
  int i;
  for (i = 0; i < NB_TEST; i++)
  {
    calib->angleMoyenTest[i] = 0;
    calib->poidsMoyenTest[i] = 0;
  }
  calib->numerosTest = 0;
}

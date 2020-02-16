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
  uint16_t angleMoyen;
  uint8_t poidsMoyen;

  uint16_t angleMoyenTest[NB_TEST];
  uint8_t poidsMoyenTest[NB_TEST];
  uint8_t numerosTest;

} T_calibration_axe;
/*===============================GLOBAL Variable====================================*/

T_calibration_axe Calibration;
T_RPM Rpm;
T_Acc_config AccConfig;
T_ACC ACC[DIMENSION]; // Ne servira plus a rien bientot
T_ACC AccMax;
T_ACC AccMin;
uint16_t RawAcc[DIMENSION]; // Sert pour les tests 


/*====================================Declaration====================================*/
void Calibration_init(T_calibration_axe *calib);

/*====================================Definition====================================*/
void Calibration_init(T_calibration_axe *calib)
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

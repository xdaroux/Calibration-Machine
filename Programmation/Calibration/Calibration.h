#pragma once
#include "RPM.h"
#include "ACCELEROMETRE.h"
/*====================================DEFINE====================================*/

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
};
//fin etat

#define NB_TEST 10

//acc
#define DIMENSION 100 // sois la dimension du tableau pour le nb de lecture dansune rotation du systeme

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

T_calibration CALIBRATION;
T_RPM RPM;
T_ACC ACC[DIMENSION];
uint16_t rawAcc[DIMENSION];

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

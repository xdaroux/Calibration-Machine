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
  PRE_CALCUL,
  TEST,
  CALCUL_INTERMEDIAIRE,
  CALCUL_MOYENNE,
  AFFICHER
}State;
//fin etat

#define NB_TEST 10
//acc
#define DIMENSION 100 // sois la dimension du tableau pour le nb de lecture dans une rotation du systeme

/*====================================TYPEDEF====================================*/
typedef struct
{
  //volatile int etat;
  uint16_t angleMoyen;
  //uint8_t poidsMoyen;

  uint16_t angleMoyenTest[NB_TEST];
  T_ACC minValue[NB_TEST];
  T_ACC maxValue[NB_TEST];
  
  //uint8_t poidsMoyenTest[NB_TEST];
  uint8_t numerosTest;

} T_calibration_axe;

typedef struct 
{
  volatile uint8_t etat = INIT;

}T_calibration;

/*===============================GLOBAL Variable====================================*/

T_calibration_axe CalibrationAxe[NOMBRE_ACC_MAX];
T_calibration Calibration;
T_RPM Rpm;
T_Acc_config AccConfig;
//T_ACC ACC[DIMENSION]; // Ne servira plus a rien bientot
T_ACC AccMax;
T_ACC AccMin;
int16_t RawAcc[DIMENSION]; // Sert pour les tests 


void algo_peak(uint16_t*tableau, int dimension, T_calibration_axe * calibAxe);



/*====================================Declaration====================================*/
void Calibration_init(T_calibration_axe *calib);
void algo_peak(int16_t*tableau, int dimension, T_calibration_axe * calibAxe);
void calibration_axe_afficher(T_calibration_axe * calibAxe);

/*====================================Definition====================================*/
void Calibration_init(T_calibration_axe *calib)
{
  calib->angleMoyen = 0;
  //calib->poidsMoyen = 0;
  int i;
  for (i = 0; i < NB_TEST; i++)
  {
    calib->angleMoyenTest[i] = 0;
    //calib->poidsMoyenTest[i] = 0;
  }
  calib->numerosTest = 0;
}

void algo_peak(int16_t*tableau, int dimension, T_calibration_axe * calibAxe)
{
  int i;
  uint16_t highestValue = 0;
  uint16_t lowestValue = 1027;

  for(i=0;i<DIMENSION;i++)
  {
    if(tableau[i] > highestValue)
    {
      highestValue = tableau[i];
      calibAxe->maxValue[calibAxe->numerosTest].count = i ;

    }

    if(tableau[i] < lowestValue)
    {
      if(tableau[i] > 0)
      {
      lowestValue = tableau[i]; 
      calibAxe->minValue[calibAxe->numerosTest].count = i ;
      }
    }
  }
  calibAxe->minValue[calibAxe->numerosTest].rawAcc = lowestValue;
  calibAxe->maxValue[calibAxe->numerosTest].rawAcc = highestValue;
  ACC_convertRawToG(&calibAxe->minValue[calibAxe->numerosTest], 520, 107);
  ACC_convertRawToG(&calibAxe->maxValue[calibAxe->numerosTest], 520, 107);
}


void calibration_axe_afficher(T_calibration_axe * calibAxe)
{
  Serial.print("Angle Moyenne : ");
  Serial.println(calibAxe->angleMoyen);

  Serial.println("| Angle |MAX count|MAX rawAcc|MAX gAcc|MIN count|MIN rawAcc|MIN gAcc");
  for(int i = 0; i< NB_TEST; i++)
  {
    
    Serial.print("\t");
    Serial.print(calibAxe->angleMoyenTest[i]);
    ACC_afficher(&calibAxe->maxValue[i]);
    ACC_afficher(&calibAxe->minValue[i]);
    Serial.print("\n");
    //Serial.print(calibAxe->maxValue[i]);
    
    //Serial.println(calibAxe->minValue[i]);
    
  }
}


void configuration_init()
{
  for(int i = 0; i <DIMENSION; i++)
  {
    RawAcc[i] = -1;

  }
  
}

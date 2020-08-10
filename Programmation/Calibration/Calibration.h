#pragma once

/*Include======================================================================*/

#include <Arduino.h>
#include "Accelerometre.h"
#include "displayData.h"
#include "RPM_NEW.h"

/*Define=======================================================================*/
#define pinSwitch 13
#define offsetPin A7
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
} State;
//fin etat

#define NB_TEST 20
//acc
#define DIMENSION 100 // sois la dimension du tableau pour le nb de lecture dans une rotation du systeme

/*====================================TYPEDEF====================================*/
typedef struct
{
  //volatile int etat;
  float angleMoyenMin;
  float angleMoyenMax;

  //uint8_t poidsMoyen;

  uint16_t angleMoyenTest[NB_TEST]; // a enlever dans pas long ...

  T_ACC minValue[NB_TEST]; //contenir les valeur de tout les test
  T_ACC maxValue[NB_TEST]; // contenir les valeur de tout les test

  T_ACC minMoyen; // faire la moyenne
  T_ACC maxMoyen; // faire la moyenne

  int minMoyenOffset; // Appliquer le offset
  int maxMoyenOffset; // Appliquer le offset


  //uint8_t poidsMoyenTest[NB_TEST];
  uint8_t numerosTest;

} T_calibration_axe;

typedef struct
{
  volatile uint8_t etat = INIT;

} T_calibration;

/*===============================GLOBAL Variable====================================*/

T_calibration_axe CalibrationAxe[NOMBRE_ACC_MAX];
T_calibration Calibration;
//T_RPM Rpm; // OLD RPM
rpm_t Rpm;
T_Acc_config AccConfig;
//T_ACC ACC[DIMENSION]; // Ne servira plus a rien bientot
T_ACC AccMax;
T_ACC AccMin;
int16_t RawAcc[DIMENSION]; // Sert pour les tests

void algo_peak(uint16_t *tableau, int dimension, T_calibration_axe *calibAxe);

/*====================================Declaration====================================*/
void Calibration_init(T_calibration_axe *calib);
void algo_peak(int16_t *tableau, int dimension, T_calibration_axe *calibAxe);
void calibration_axe_afficher(T_calibration_axe *calibAxe);
void calibration_axe_calcul_moyenne_angle(T_calibration_axe *calibAxe, uint16_t nb_test);
void calibration_applique_offset(T_calibration_axe *calibAxe);

/*====================================Definition====================================*/
void Calibration_init(T_calibration_axe *calib)
{
  calib->angleMoyenMin = 0;
  calib->angleMoyenMax = 0;  //calib->poidsMoyen = 0;
  int i;
  for (i = 0; i < NB_TEST; i++)
  {
    calib->angleMoyenTest[i] = 0;
    //calib->poidsMoyenTest[i] = 0;
  }
  calib->numerosTest = 0;
}

void algo_peak(int16_t *tableau, int dimension, T_calibration_axe *calibAxe)
{
  int i;
  uint16_t highestValue = 0;
  uint16_t lowestValue = 1027;

  for (i = 0; i < DIMENSION; i++)
  {
    if (tableau[i] > highestValue)
    {
      highestValue = tableau[i];
      calibAxe->maxValue[calibAxe->numerosTest].count = i;
    }

    if (tableau[i] < lowestValue)
    {
      if (tableau[i] > 0)
      {
        lowestValue = tableau[i];
        calibAxe->minValue[calibAxe->numerosTest].count = i;
      }
    }
  }
  calibAxe->minValue[calibAxe->numerosTest].rawAcc = lowestValue;
  calibAxe->maxValue[calibAxe->numerosTest].rawAcc = highestValue;
  ACC_convertRawToG(&calibAxe->minValue[calibAxe->numerosTest], 520, 107);
  ACC_convertRawToG(&calibAxe->maxValue[calibAxe->numerosTest], 520, 107);
}

void calibration_axe_afficher(T_calibration_axe *calibAxe)
{
  //Serial.print("Angle Moyenne : ");
  //Serial.println(calibAxe->angleMoyen);

  Serial.println("MAX count|MAX rawAcc|MAX gAcc|MIN count|MIN rawAcc|MIN gAcc");
  for (int i = 0; i < NB_TEST; i++)
  {

    Serial.print("\t");
    ACC_afficher(&calibAxe->maxValue[i]);
    ACC_afficher(&calibAxe->minValue[i]);
    Serial.print("\n");
  }
  Serial.println("------------------------------Moyenne------------------------------");
  Serial.print("\t");
  ACC_afficher(&calibAxe->maxMoyen);
  ACC_afficher(&calibAxe->minMoyen);
  Serial.print("\n");
  Serial.print("Angle Moyen MAX : ");
  Serial.println(calibAxe->angleMoyenMax);
  Serial.print("Angle Moyen MIN : ");
  Serial.println(calibAxe->angleMoyenMin);
   Serial.print("Angle diff : ");
  Serial.println(calibAxe->angleMoyenMin - calibAxe->angleMoyenMax);

}

void configuration_init()
{
  for (int i = 0; i < DIMENSION; i++)
  {
    RawAcc[i] = -1;
  }
}
void calibration_axe_calcul_moyenne_angle(T_calibration_axe *calibAxe, uint16_t nb_test)
{
  int i = 0;
  int tmpRaw = 0;
  float tmpGacc = 0;
  int tmpCount = 0; //angle moyen
  // max value
  for (i = 0; i < nb_test; i++)
  {
    tmpCount += calibAxe->maxValue[i].count;
    tmpRaw += calibAxe->maxValue[i].rawAcc;
    tmpGacc += calibAxe->maxValue[i].gAcc;
  }

  calibAxe->maxMoyen.count = tmpCount / nb_test;
  calibAxe->maxMoyen.rawAcc = tmpRaw / nb_test;
  calibAxe->maxMoyen.gAcc = tmpGacc / nb_test;

  tmpRaw = 0;
  tmpGacc = 0;
  tmpCount = 0; 
  //min value
  for (i = 0; i < nb_test; i++)
  {
    tmpCount += calibAxe->minValue[i].count;
    tmpRaw += calibAxe->minValue[i].rawAcc;
    tmpGacc += calibAxe->minValue[i].gAcc;
  }

  calibAxe->minMoyen.count = tmpCount / nb_test;
  calibAxe->minMoyen.rawAcc = tmpRaw / nb_test;
  calibAxe->minMoyen.gAcc = tmpGacc / nb_test;

  //calcul angle moyenne 
  
  calibAxe->angleMoyenMax = calibAxe->maxMoyen.count * (3.6); // 3.6 360/DIMENSION
  calibAxe->angleMoyenMin = calibAxe->minMoyen.count * (3.6); //360/DIMENSION

  calibration_applique_offset(calibAxe);


}

void calibration_applique_offset(T_calibration_axe *calibAxe)
{
  int offset = 0;
       //offset  
    offset = analogRead(offsetPin);
    offset = map(offset,0,1023,-50,50);
    /*Serial.print("offset : ");
    Serial.println(offset);*/

  calibAxe->minMoyenOffset = calibAxe->minMoyen.count + offset;
  calibAxe->maxMoyenOffset = calibAxe->maxMoyen.count + offset;

  if(calibAxe->minMoyenOffset > 100)
  {
    calibAxe->minMoyenOffset -= 100;
  }
  else if(calibAxe->minMoyenOffset < 0)
  {
    calibAxe->minMoyenOffset += 100;
  }


    if(calibAxe->maxMoyenOffset > 100)
  {
    calibAxe->maxMoyenOffset -= 100;
  }
  else if(calibAxe->maxMoyenOffset < 0)
  {
    calibAxe->maxMoyenOffset += 100;
  }
}

#pragma once
/*Include =====================================================================*/

#include <Arduino.h>
/*Pin Define  =================================================================*/
#define RPMpin 2

/*Define  ====================================DEFINE===========================*/
/*----------------RPM.ETAT----------------*/
enum
{
  ZERO,
  AUGMENTE,
  STABLE,
  DIMINUE
}rpmState;
//Nombre de RPM enregistre dans le passer
#define NB_RPM 5        // nombre de test enregistre pour calcul de la moyenne //jou sur le temps de stabilisation
#define RPM_MIN 200     //RPM minimum pour considere dans un autre etat que ZERO
#define RPM_DIF_MAX 200 // sensibilite du RPM pour les etats

//Temps entre calcul de rpm
#define TEMPS_CALCUL 500 //500 ms entre chaque calcul de RPM

/*====================================TYPEDEF====================================*/

typedef struct
{
  unsigned int leRpm;       //RPM actuelle
  unsigned int rpm[NB_RPM]; //enregistre

  unsigned int nbRotation; //Pour l'interup

  int etat; //ZERO,AUGMENTE,STABLE,DIMINUE

  unsigned long timerOld;

} T_RPM;

/*====================================Declaration===================================*/
void RPM_init(T_RPM *);
void RPM_calcul(T_RPM *);
void RPM_enregistre(T_RPM *);
void RPM_etat(T_RPM *);
void RPM_incrementeRotation(T_RPM *);
void RPM_main(T_RPM *);



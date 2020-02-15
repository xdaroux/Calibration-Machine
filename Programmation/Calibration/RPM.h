#pragma once
/*Include =====================================================================*/
#include <Arduino.h>
/*Pin Define  =================================================================*/
#define pinRPM 2 //pin digital 2 Arduino input

/*Define  =====================================================================*/
/*----------------RPM.ETAT----------------*/
enum
{
  ZERO,
  AUGMENTE,
  STABLE,
  DIMINUE
}rpmState;
//Nombre de RPM enregistre dans le passer
#define NB_RPM_TEST 5        // nombre de test enregistre pour calcul de la moyenne //jou sur le temps de stabilisation
#define RPM_MIN 200     //RPM minimum pour considere dans un autre etat que ZERO
#define RPM_DIF_MAX 200 // sensibilite du RPM pour les etats

//Temps entre calcul de rpm
#define TEMPS_CALCUL 500 //500 ms entre chaque calcul de RPM

/*typedef =====================================================================*/

typedef struct
{
  uint16_t leRpm;       //RPM actuelle
  uint16_t rpm[NB_RPM_TEST]; //enregistre

  volatile uint32_t nbRotation; //Pour sommer dans  l'interup

  uint8_t etat; //ZERO,AUGMENTE,STABLE,DIMINUE

  uint32_t timerOld; //Pour calcul RPM

} T_RPM;

/*Declaration des fonctions Public  ===========================================*/
void Rpm_config(void);
void RPM_init(T_RPM *);
void RPM_calcul(T_RPM *);
void RPM_enregistre(T_RPM *);
void RPM_etat(T_RPM *);
void RPM_incrementeRotation(T_RPM *);
void RPM_main(T_RPM *);



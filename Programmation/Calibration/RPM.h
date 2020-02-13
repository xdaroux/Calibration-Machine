#pragma once

/*====================================PIN OUT====================================*/
#define RPMpin 2

/*====================================DEFINE====================================*/
/*----------------RPM.ETAT----------------*/
enum
{
  ZERO,
  AUGMENTE,
  STABLE,
  DIMINUE
};
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

/*====================================Definition====================================*/
//init
void RPM_init(T_RPM *rpm)
{
  /* Set up RPM*/
  pinMode(RPMpin, INPUT_PULLUP);
  //la partie qui suit commenter dois etre dans le set up du main setup
  //attachInterrupt(digitalPinToInterrupt(RPMpin), blink, RISING); // Can use HIGHT,LOW,CHANGE,RISING,FALLING
  /*FIN set up RPM*/

  int i;
  rpm->leRpm = 0;
  for (i = 0; i < NB_RPM; i++)
  {
    rpm->rpm[i] = 0;
  }

  rpm->nbRotation = 0;
  rpm->etat = ZERO;
  rpm->timerOld = millis();
}

//calcul
void RPM_calcul(T_RPM *rpm)
{
  //nombre de roation
  unsigned int nbRotation = rpm->nbRotation; // enregistre local le nombre de rotation
  rpm->nbRotation = 0;                       //mettre tous de suite a zero, car pourrais etre appeller a tout moment

  //timer
  unsigned long timer = millis();
  unsigned long timerDif = timer - rpm->timerOld;
  rpm->timerOld = timer;

  //enregistrer
  RPM_enregistre(rpm); // enregistre le dernier rpm calculer

  //calcul
  rpm->leRpm = (nbRotation * 60) / (timerDif * 0.001); //Rotation / minute : (NombreRotation/timerDif msec)*1000msec * 60sec //version optimiser 0.06

  //etat
  RPM_etat(rpm);
}

//enregistrer
void RPM_enregistre(T_RPM *rpm)
{
  int i;

  for (i = (NB_RPM - 1); i > 0; i--)
  {
    rpm->rpm[i] = rpm->rpm[i - 1];
  }
  rpm->rpm[0] = rpm->leRpm;
}

//etat
void RPM_etat(T_RPM *rpm)
{
  if (rpm->leRpm > RPM_MIN) // si le rpm actuelle n'est pas = a 0 fait les autre test
  {
    int i;
    int rpmMoyen = 0;
    int rpmSomme = 0;
    int rpmDif = 0;

    // caclul pour differencier l'etat
    for (i = 0; i < NB_RPM; i++)
    {
      rpmSomme += rpm->rpm[i];
    }
    rpmMoyen = rpmSomme / NB_RPM;
    rpmDif = rpmMoyen - rpm->leRpm;
    /* Serial.print(rpmDif);*/

    // Trouver l'etat
    if (rpmDif <= -RPM_DIF_MAX)
    {
      rpm->etat = AUGMENTE;
    }
    else if (rpmDif >= RPM_DIF_MAX)
    {
      rpm->etat = DIMINUE;
    }
    else if (rpmDif > -RPM_DIF_MAX && rpmDif < RPM_DIF_MAX) // si le RPM difference est de 200 donc correcte STALE
    {
      rpm->etat = STABLE;
    }
  }
  else
  {
    rpm->etat = ZERO;
  }
  /*Serial.print("\t");
  Serial.println(rpm->etat);
  Serial.println("...");*/
}

void RPM_main(T_RPM *rpm)
{
  if (millis() - rpm->timerOld > TEMPS_CALCUL) // En rentre dans RPM_calcul chaque 1000 mSecondes
  {
    RPM_calcul(rpm);

    /*Serial.println("-");
    Serial.print("Etat :");
    Serial.println(rpm->etat);
    Serial.print("RPM  :");
    Serial.println(rpm->leRpm);*/
  }
}

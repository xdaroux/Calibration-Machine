#include "Rpm.h"


/*Definition des fonctions  ===================================================*/
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
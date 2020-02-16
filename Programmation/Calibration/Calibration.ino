

#include "Calibration.h"

#define DEBUG 0

// un calibration comporte juste un accelerometre donc on fait un tableau de calibration si on a 3 calibration a faire [3];
//je vais m'arranger pour que sa le detecte au debut lors de l'alimentation quick connecte qui short une pin pour detecter !
//et mettre le bon nombre dans lel tableau en foction de sa !
volatile uint8_t FlagTest = 0;
uint32_t timerOlderMicros = micros();

unsigned long timerSwitch = millis();

#define pinSwitch 13


double timeEntreMesure;         //Temps entre chaque lecture analog rn fonction du Rpm

void setup()
{
  Serial.begin(9600);
  
  //Rpm
  Rpm_config();
  attachInterrupt(digitalPinToInterrupt(pinRPM), blink, RISING);
  RPM_init(&Rpm);

  pinMode(pinSwitch, INPUT_PULLUP);



  //calibration
  Calibration_init(&Calibration);

  //acceleration
  analogReference(EXTERNAL); // IMPORTANT 3.3V por cet accelerometre
  ACC_init_tableau(&ACC[0], DIMENSION);
  Acc_config_pin(); //Configurer les digitals pin qui detecte l'activation des Accelerometres
  Acc_config_init(&AccConfig);  // Initialise des parametres de Acc_config

  /*
  ici il va falloir faire une condition pour savoir combien acc/l/rometre
   son connecter eb shortant un digtal pin quand le connecteur est mis
   pour incrementer number Acc
  */

  //init tableau vite

}

void loop()
{
  //Variable
  uint8_t i = 0;

  //Calcul RPM
  RPM_main(&Rpm);

  switch (Calibration.etat)
  {
  /*INIT  =====================================================================*/
  case INIT:
    
    //Regarde les connection des accelerometres si ont changer
    Acc_config_change(&AccConfig);
    //Gestion du bouton test
    if (millis() - timerSwitch > 250)
    {
      if (Rpm.leRpm > RPM_MIN)
      {
        if (digitalRead(pinSwitch) == 0)
        {
          Calibration.etat = START;
          timerSwitch = millis();
        }
      }
    }
    break;
  /*START =====================================================================*/
  case START:
    Serial.println("START");

    // ici faut j'att que le sensor passe devant appel interupt fontion qui amener par lui meme l'etat a TEST
    // je veux faire au minimum dix test par capteur ! donc sa va alterner entre STABLE et TEST 10 fois accumuler les moyennes !
    // calcul pour 2 degree itteration

    timeEntreMesure = (double)(((1 / ((double)Rpm.leRpm / 60)) / DIMENSION) * 1000000); // 60 conversiont RPM par RPS, 1 passer de HZ e Periode, DIMENSION nb de test sur 360 degree
    Calibration.etat = TEST;

    break;
  case TEST:
  {
    i = 0;
    //Serial.println("TEST");
    // calibraion.angleMoyenTest[calibration.numerosTest] = test(); // faire le test s'incroniser ! avec la rotation
    while (Calibration.etat != CALCUL_INTERMEDIAIRE)
    {

      while (FlagTest == 1 && i < DIMENSION)
      {

        if (micros() - timerOlderMicros > timeEntreMesure)
        {

          timerOlderMicros = micros();
          ACC_read(&ACC[i], ACC_0, i);
          i++;
        }
      }
    }
  }
  break;

  case CALCUL_INTERMEDIAIRE:
  {
    Serial.println("CALCUL_INTERMEDIAIRE");
    i = 0;
    //calcul du peak
    Serial.print("RPM : ");
    Serial.println(Rpm.leRpm);
    Serial.print("TIME entre Mesure : ");
    Serial.println(timeEntreMesure);
    Serial.println("================");

    for (i = 0; i < DIMENSION - 1; i++)
    {
      ACC_convertRawToG(&ACC[i], 520, 107);
      Serial.println(ACC[i].gAcc);
    }
    Serial.println("================");
    Calibration.etat = CALCUL;
  }
  break;
  case CALCUL: // calcul de la moyenne
    Serial.println("CALCUL");
    AccConfig.testAccNum++; //Passer a la prochaine pin
    Calibration.etat = INIT;
    break;
  case AFFICHER:
    Serial.println("AFFICHER");
    // vas gerer le systeme d<affichage future que je ne connait pas encore
    //j'aimerais des 7 segment version geante qui affichye un chiffre en 0 et 12
    break;
  default:
    //CALIBRATION.etat = INIT;
    //Serial.print("default");
    break;
  }
}

/*====================================Interupt====================================*/
void blink()
{

  Rpm.nbRotation++;

  if (Calibration.etat == TEST)
  {
    if (FlagTest == 0)
    {
      FlagTest++;
    }
    else
    {
      FlagTest--;
      Calibration.etat = CALCUL_INTERMEDIAIRE;
    }
  }
}



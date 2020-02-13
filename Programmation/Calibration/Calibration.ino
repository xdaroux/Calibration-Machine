

#include "Calibration.h"

// un calibration comporte juste un accelerometre donc on fait un tableau de calibration si on a 3 calibration a faire [3];
//je vais m'arranger pour que sa le detecte au debut lors de l'alimentation quick connecte qui short une pin pour detecter !
//et mettre le bon nombre dans lel tableau en foction de sa !
volatile unsigned int flagTest = 0;
unsigned long timerOlderMicros = micros();
unsigned long timer[DIMENSION];
double timeEntreMesure;
unsigned long timerStartInterupt;
unsigned long timerStopInterupt;

unsigned long timerSwitch = millis();

#define pinSwitch 13

uint8_t numberAcc = 4;    // va falloir que je fasse varier se chiffre en  fonction des input output 
uint8_t testAccNum = 0;   // Savoir qu'elle accelerometre on test

void setup()
{
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(RPMpin), blink, RISING);
  //rpm
  RPM_init(&RPM);

  pinMode(pinSwitch, INPUT_PULLUP);

  //calibration
  Calibration_init(&CALIBRATION);

  //acceleration
  analogReference(EXTERNAL); // IMPORTANT 3.3V por cet accelerometre
  ACC_init_tableau(&ACC[0], DIMENSION);
}

void loop()
{
  //RPM calcul
  RPM_main(&RPM); // TJRS le faire, car utile pour tout le reste
  //calibration

  //Serial.println(CALIBRATION.etat);
  if (millis() - timerSwitch > 250)
  {
    if (RPM.leRpm > 500)
    {
      if (digitalRead(pinSwitch) == 0)
      {
        CALIBRATION.etat = START;
        timerSwitch = millis();
      }
    }
  }
  switch (CALIBRATION.etat)
  {
  case START:
    Serial.println("START");

    // ici faut j'att que le sensor passe devant appel interupt fontion qui amener par lui meme l'etat a TEST
    // je veux faire au minimum dix test par capteur ! donc sa va alterner entre STABLE et TEST 10 fois accumuler les moyennes !
    // calcul pour 2 degree itteration

    timeEntreMesure = (double)(((1 / ((double)RPM.leRpm / 60)) / DIMENSION) * 1000000); // 60 conversiont RPM par RPS, 1 passer de HZ e Periode, DIMENSION nb de test sur 360 degree
    CALIBRATION.etat = TEST;

    break;
  case TEST:
  {
    int i = 0;
    //Serial.println("TEST");
    // calibraion.angleMoyenTest[calibration.numerosTest] = test(); // faire le test s'incroniser ! avec la rotation
    while (CALIBRATION.etat != CALCUL_INTERMEDIAIRE)
    {

      while (flagTest == 1 && i < DIMENSION)
      {

        if (micros() - timerOlderMicros > timeEntreMesure)
        {

          timerOlderMicros = micros();
          ACC_read(&ACC[i], ACC_0, i);
          //timer[i] = timerOlderMicros;
          i++;
        }
      }
    }
  }
  break;

  case CALCUL_INTERMEDIAIRE:
  {
    Serial.println("CALCUL_INTERMEDIAIRE");
    int i;
    //calcul du peak
    Serial.print("RPM : ");
    Serial.println(RPM.leRpm);
    Serial.print("TIME entre Mesure : ");
    Serial.println(timeEntreMesure);
    Serial.println("================");

    for (i = 0; i < DIMENSION - 1; i++)
    {
      ACC_convertRawToG(&ACC[i], 520, 107);
      Serial.println(ACC[i].gAcc);
      // ACC_afficher(&ACC[i]);
      //timer[i+1] = timer[i+1] - timer[0];
      // Serial.print("\t");
      //Serial.print(timer[i]);
    }
    Serial.println("================");
    CALIBRATION.etat = CALCUL;
  }
  break;
  case CALCUL:
    Serial.println("CALCUL");
    CALIBRATION.etat = INIT;
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

  RPM.nbRotation++;

  if (CALIBRATION.etat == TEST)
  {
    if (flagTest == 0)
    {
      // timerStartInterupt = micros();
      flagTest++;
    }
    else
    {

      flagTest--;
      CALIBRATION.etat = CALCUL_INTERMEDIAIRE;
    }
  }
}



#include "Calibration.h"
#define DEBUG 0
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
#define NOMBRE_ACC_MAX 5

uint8_t numberAccConnected = 0; //Nombre d'accelerometre connecter
uint8_t testAccNum = 0;         // Savoir qu'elle accelerometre on test

uint8_t pinAnalogReadSequence[NOMBRE_ACC_MAX]; // faut je les mettre tous a -1 si non
int8_t pinAccConnected[NOMBRE_ACC_MAX];        // Etat des pin detection accelerometre peut oprimiser avec de bit faut init a 0
void setup()
{
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(RPMpin), blink, RISING);
  //rpm
  RPM_init(&RPM);

  pinMode(pinSwitch, INPUT_PULLUP);

  pinMode(pin_ACC_0_CONNECTED, INPUT_PULLUP);
  pinMode(pin_ACC_1_CONNECTED, INPUT_PULLUP);
  pinMode(pin_ACC_2_CONNECTED, INPUT_PULLUP);
  pinMode(pin_ACC_3_CONNECTED, INPUT_PULLUP);
  pinMode(pin_ACC_4_CONNECTED, INPUT_PULLUP);

  //calibration
  Calibration_init(&CALIBRATION);

  //acceleration
  analogReference(EXTERNAL); // IMPORTANT 3.3V por cet accelerometre
  ACC_init_tableau(&ACC[0], DIMENSION);

  /*
  ici il va falloir faire une condition pour savoir combien acc/l/rometre
   son connecter eb shortant un digtal pin quand le connecteur est mis
   pour incrementer number Acc
  */

  //init tableau vite
  pinAccConnected[ACC_0] = HIGH;
  pinAccConnected[ACC_1] = HIGH;
  pinAccConnected[ACC_2] = HIGH;
  pinAccConnected[ACC_3] = HIGH;
  pinAccConnected[ACC_4] = HIGH;
}

void loop()
{
  //Variable
  int8_t tmp = 0;
  int8_t i = 0;

  //Calcul RPM
  RPM_main(&RPM); 


  switch (CALIBRATION.etat)
  {
  /*INIT  =====================================================================*/
  case INIT:
    if (DEBUG)
    {
      Serial.println("INIT");
      delay(1000);
      Serial.print(digitalRead(pin_ACC_0_CONNECTED));
      Serial.print("  !=  ");
      Serial.println(pinAccConnected[ACC_0]);
      Serial.print(digitalRead(pin_ACC_1_CONNECTED));
      Serial.print("  !=  ");
      Serial.println(pinAccConnected[ACC_1]);
      Serial.print(digitalRead(pin_ACC_2_CONNECTED));
      Serial.print("  !=  ");
      Serial.println(pinAccConnected[ACC_2]);
      Serial.print(digitalRead(pin_ACC_3_CONNECTED));
      Serial.print("  !=  ");
      Serial.println(pinAccConnected[ACC_3]);
      Serial.print(digitalRead(pin_ACC_4_CONNECTED));
      Serial.print("  !=  ");
      Serial.println(pinAccConnected[ACC_4]);
    }
    //Gestion nombre Acc connecter

    // si un de ses condition est vrai, on refait le mapping

    if (digitalRead(pin_ACC_0_CONNECTED) != pinAccConnected[ACC_0])
    {
      if (DEBUG)
      {
        Serial.println("Etat Acc 0 Modifier");
      }
      tmp = -1;
    }
    if (digitalRead(pin_ACC_1_CONNECTED) != pinAccConnected[ACC_1])
    {
      if (DEBUG)
      {
        Serial.println("Etat Acc 1 Modifier");
      }
      tmp = -1;
    }
    if (digitalRead(pin_ACC_2_CONNECTED) != pinAccConnected[ACC_2])
    {
      if (DEBUG)
      {
        Serial.println("Etat Acc 2 Modifier");
      }
      tmp = -1;
    }
    if (digitalRead(pin_ACC_3_CONNECTED) != pinAccConnected[ACC_3])
    {
      if (DEBUG)
      {
        Serial.println("Etat Acc 3 Modifier");
      }
      tmp = -1;
    }
    if (digitalRead(pin_ACC_4_CONNECTED) != pinAccConnected[ACC_4])
    {
      if (DEBUG)
      {
        Serial.println("Etat Acc 4 Modifier");
      }
      tmp = -1;
    }

    //mapping
    if (DEBUG)
    {
      Serial.print("tmp : ");
      Serial.println(tmp);
    }

    if (tmp == -1)
    {
      numberAccConnected = 0;

      if (digitalRead(pin_ACC_0_CONNECTED) == LOW)
      {
        pinAnalogReadSequence[numberAccConnected] = pin_ACC_0;
        numberAccConnected++;
        pinAccConnected[ACC_0] = LOW;
      }
      else
      {
        pinAnalogReadSequence[numberAccConnected] = -1;
        pinAccConnected[ACC_0] = HIGH;
      }

      if (digitalRead(pin_ACC_1_CONNECTED) == LOW)
      {
        pinAnalogReadSequence[numberAccConnected] = pin_ACC_1;
        numberAccConnected++;
        pinAccConnected[ACC_1] = LOW;
      }
      else
      {
        pinAnalogReadSequence[numberAccConnected] = -1;
        pinAccConnected[ACC_1] = HIGH;
      }

      if (digitalRead(pin_ACC_2_CONNECTED) == LOW)
      {
        pinAnalogReadSequence[numberAccConnected] = pin_ACC_2;
        numberAccConnected++;
        pinAccConnected[ACC_2] = LOW;
      }
      else
      {
        pinAnalogReadSequence[numberAccConnected] = -1;
        pinAccConnected[ACC_2] = HIGH;
      }

      if (digitalRead(pin_ACC_3_CONNECTED) == LOW)
      {
        pinAnalogReadSequence[numberAccConnected] = pin_ACC_3;
        numberAccConnected++;
        pinAccConnected[ACC_3] = LOW;
      }
      else
      {
        pinAnalogReadSequence[numberAccConnected] = -1;
        pinAccConnected[ACC_3] = HIGH;
      }

      if (digitalRead(pin_ACC_4_CONNECTED) == LOW)
      {
        pinAnalogReadSequence[numberAccConnected] = pin_ACC_4;
        numberAccConnected++;
        pinAccConnected[ACC_4] = LOW;
      }
      else
      {
        pinAnalogReadSequence[numberAccConnected] = -1;
        pinAccConnected[ACC_4] = HIGH;
      }
      if (DEBUG)
      {
        Serial.print("Number ACC Connected : ");
        Serial.println(numberAccConnected);

        for (i = 0; i < NOMBRE_ACC_MAX; i++)
        {
          Serial.print("Valeur dans sequence de pin :");
          Serial.println(pinAnalogReadSequence[i]);
        }
      }
      tmp = 0;
    } // fin mapping

    //Gestion du bouton test
    if (millis() - timerSwitch > 250)
    {
      if (RPM.leRpm > RPM_MIN)
      {
        if (digitalRead(pinSwitch) == 0)
        {
          CALIBRATION.etat = START;
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

    timeEntreMesure = (double)(((1 / ((double)RPM.leRpm / 60)) / DIMENSION) * 1000000); // 60 conversiont RPM par RPS, 1 passer de HZ e Periode, DIMENSION nb de test sur 360 degree
    CALIBRATION.etat = TEST;

    break;
  case TEST:
  {
    i = 0;
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
    i = 0;
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
  case CALCUL: // calcul de la moyenne
    Serial.println("CALCUL");
    testAccNum++; //Passer a la prochaine pin
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

//J'ai ajouter se commentaire pour fin de test avec mon laptop voir comment pull sur mon ordi !!!! 

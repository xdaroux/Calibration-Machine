

#include "Calibration.h"

#define DEBUG 0
#define SIMPLE_DEBUG 0

// un calibration comporte juste un accelerometre donc on fait un tableau de calibration si on a 3 calibration a faire [3];
//je vais m'arranger pour que sa le detecte au debut lors de l'alimentation quick connecte qui short une pin pour detecter !
//et mettre le bon nombre dans lel tableau en foction de sa !
volatile uint8_t FlagTest = 0;
uint32_t timerOlderMicros = micros();

unsigned long timerSwitch = millis();
unsigned long timerAfficherRPM = millis();




double timeEntreMesure; //Temps entre chaque lecture analog rn fonction du Rpm

/*Display*********************/
   extern  TM1637 RpmDisplay;
   extern  TM1637 Acc0Display;
   extern  TM1637 Acc1Display;
   extern  TM1637 Acc2Display;
   extern  TM1637 Acc3Display;

void setup()
{
  Serial.begin(115200);

  //Rpm
 // Rpm_config();
  rpm_init(&Rpm);
  attachInterrupt(digitalPinToInterrupt(pinRPM), blink, FALLING); // changement plus precis dans le falling que rising se type de senseur !
 


  pinMode(pinSwitch, INPUT_PULLUP);

  //calibration
  Calibration_init(CalibrationAxe);

  //acceleration
  analogReference(EXTERNAL); // IMPORTANT 3.3V por cet accelerometre
  //ACC_init_tableau(&ACC[0], DIMENSION);
  Acc_config_pin();            //Configurer les digitals pin qui detecte l'activation des Accelerometres
  Acc_config_init(&AccConfig); // Initialise des parametres de Acc_config

  //calibration
/*dISPLAY */
  setup_displayData();
  


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
  
  if(millis() - timerAfficherRPM > 500)
  {
    rpm_calcul(&Rpm);
    uptade_display_rpm(RpmDisplay,Rpm.rpm);
    timerAfficherRPM = millis();


  }

  switch (Calibration.etat)
  {
  /*INIT  =====================================================================*/
  case INIT:
   
    //Regarde les connection des accelerometres si ont changer
    Acc_config_change(&AccConfig);

    // remet tout les compteurs a Zero
    AccConfig.testAccNum = 0;
    for (i = 0; i < NOMBRE_ACC_MAX; i++)
    {
      CalibrationAxe[i].numerosTest = 0;
    }

    //Gestion du bouton test
    if (millis() - timerSwitch > 250)
    {
      if (Rpm.rpm > RPM_MIN)
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
    if (DEBUG)
    {
      Serial.println("=============================");
      Serial.println("START");
      Serial.print("Acc # : ");
      Serial.println(AccConfig.testAccNum);
      Serial.print("Test # : ");
      Serial.println(CalibrationAxe[AccConfig.testAccNum].numerosTest);
      // Gestion e quelle acc on est rendu !!!
    }

    Calibration.etat = PRE_CALCUL;
    break;
  case PRE_CALCUL:
    if (DEBUG)
    {
      Serial.println("PRE_CALCUL");
    }

    timeEntreMesure = (double)(((1 / ((double)Rpm.rpm / 60)) / DIMENSION) * 1000000); // 60 conversiont RPM par RPS, 1 passer de HZ e Periode, DIMENSION nb de test sur 360 degree
    Calibration.etat = TEST;

    break;
  case TEST:
  {
    i = 0;
    if(DEBUG)
    {
    Serial.println("TEST");
    }
    // calibraion.angleMoyenTest[calibration.numerosTest] = test(); // faire le test s'incroniser ! avec la rotation
    while (Calibration.etat != CALCUL_INTERMEDIAIRE)
    {

      while (FlagTest == 1 && i < DIMENSION)
      {

        if (micros() - timerOlderMicros > timeEntreMesure)
        {

          timerOlderMicros = micros();
          // ACC_read(&ACC[i], ACC_0, i);
          ACC_read_raw_acc(&RawAcc[i], AccConfig.pinAnalogReadSequence[AccConfig.testAccNum], DIMENSION);
          i++;
        }
      }
    }
  }
  break;

  case CALCUL_INTERMEDIAIRE:
  {
    if (DEBUG)
    {
      Serial.println("CALCUL_INTERMEDIAIRE");
      ACC_affichier_raw_acc(RawAcc, DIMENSION); // affiche le data lu en raw
    }

    // trouver les peak min et max    &
    //enregistre les valuer min max dans calibration.Axe.nbtest
    algo_peak(RawAcc, DIMENSION, &CalibrationAxe[AccConfig.testAccNum]);

    CalibrationAxe[AccConfig.testAccNum].numerosTest++;
    if (CalibrationAxe[AccConfig.testAccNum].numerosTest < NB_TEST)
    {
      Calibration.etat = START;
    }
    else
    {
      //calibration_axe_afficher(&CalibrationAxe[AccConfig.testAccNum]);
      Calibration.etat = CALCUL_MOYENNE;
    }
  }
  break;
  case CALCUL_MOYENNE: // calcul de la moyenne
    Serial.println("CALCUL MOYENNE");
    Serial.print("Valeur ACC # ");
    Serial.println(AccConfig.testAccNum);

    calibration_axe_calcul_moyenne_angle(&CalibrationAxe[AccConfig.testAccNum], NB_TEST);
    calibration_axe_afficher(&CalibrationAxe[AccConfig.testAccNum]);



    AccConfig.testAccNum++; //Passer a la prochaine pin
    if(AccConfig.testAccNum < AccConfig.numberAccConnected)
    {

      Calibration.etat = START;
    }
    else
    {
      Calibration.etat = AFFICHER;
    }
    
    
    break;
  case AFFICHER:
    Serial.println("AFFICHER");

  uptade_display_acc(Acc0Display,CalibrationAxe[0].maxMoyen.count,CalibrationAxe[0].minMoyen.count);
  uptade_display_acc(Acc1Display,CalibrationAxe[1].maxMoyen.count,CalibrationAxe[1].minMoyen.count);
  uptade_display_acc(Acc2Display,CalibrationAxe[2].maxMoyen.count,CalibrationAxe[2].minMoyen.count);
  uptade_display_acc(Acc3Display,CalibrationAxe[3].maxMoyen.count,CalibrationAxe[3].minMoyen.count);
  //RPM
  
    // vas gerer le systeme d'affichage future que je ne connait pas encore
    //j'aimerais des 7 segment version geante qui affichye un chiffre en 0 et 12
    Calibration.etat = INIT;
    break;
  default:
    Calibration.etat = INIT;
    //Serial.print("default");
    break;
  }
}

/*Interupt lier avec le RPM=====================================================================*/
void blink()
{
	//Pour la suite de test des ACC
  if(Calibration.etat == TEST)
  {
    if(FlagTest == 0)
    {
      FlagTest++;
    }
    else
    {
       FlagTest--;
      Calibration.etat = CALCUL_INTERMEDIAIRE;
    }
  }
  //Pour le RPM 
	Rpm.timerOlder = Rpm.timer;
	Rpm.timer = micros();
}

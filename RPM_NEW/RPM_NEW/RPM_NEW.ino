#define NB_RPM 5	//Nombre de rpm enregistre dans le tableau
#define MICROStoSECONDE 0.000001	  // Multiplication pour transformer des uSec en S 	
#define SECONDEtoMINUTE 60
#define MICROStoMINUTE  0.00006

#define DELAY_ZERO_RPM 600000 // 0.6 secondes environ 100 Rpm

#define DEBUG 0

typedef struct
{
	uint16_t rpm;

	volatile uint32_t timerOlder;
	volatile uint32_t timer;

	uint16_t rpmTableau[NB_RPM];
	uint8_t count;

} rpm_t;

 rpm_t Rpm;

#define pinRPM 2
void setup()
{
	Serial.begin(9600);
	attachInterrupt(digitalPinToInterrupt(pinRPM), blink, FALLING);
	rpm_init(&Rpm);
}

void loop()
{

	//Serial.print(rpm_calcul2());
	//Serial.print("\t");
	delay(500);
	rpm_calcul(&Rpm);
	Serial.println(Rpm.rpm);
}

void blink()
{
	//Faut tjrs le faire Meme si on le calcul pas
	Rpm.timerOlder = Rpm.timer;
	Rpm.timer = micros();
}
void rpm_init(rpm_t *rpm)
{
	int i = 0;

	rpm->rpm = 0;
	rpm->timer = micros();
	rpm->timerOlder = micros();
	rpm->count = 0;
	for (i = 0; i < NB_RPM; i++)
	{
		rpm->rpmTableau[i] = 0;
	}
}
void rpm_calcul(rpm_t *rpm)
{
	int i = 0;
	uint16_t tmpRpm = rpm->rpm;
	uint32_t sommeRpm = 0;
	uint32_t temps = rpm->timer - rpm->timerOlder; // Enregistre local car peut changer a tout moment
	//calcul
	if (temps != 0) //eviter les division par 0
	{
		tmpRpm = (uint32_t)((1 / ((temps)*MICROStoSECONDE)) * SECONDEtoMINUTE);
	}

	// Si il n'y a aucun uptate de valeur depuis 0.6 seconde sois 100Hz RPM = 0
	if (micros() - rpm->timer > DELAY_ZERO_RPM)
	{
		tmpRpm = 0;
	}

	//Mettre la valeur dans le Tableau RPM
	if (rpm->count == NB_RPM) //Remttre a 0 dans le count du tableau si on a attein le max
	{
		rpm->count = 0;
	}

	rpm->rpmTableau[rpm->count] = tmpRpm;
	rpm->count++;
	//Sommer le tableau de RPM 
	for (i = 0; i < NB_RPM; i++)
	{
		sommeRpm = sommeRpm + rpm->rpmTableau[i];
	}


	//Enregistre le vrai RPM en moyenne 
	rpm->rpm = sommeRpm / NB_RPM;

	if (DEBUG)
	{
		Serial.print("temps Entre 2 Interupt : ");
		Serial.println(temps);
		Serial.print("tmpRpm :");
		Serial.println(tmpRpm);
		Serial.print("SommeRpm :");
		Serial.println(sommeRpm);
		Serial.print("Le RPM : ");
		Serial.println(rpm->rpm);
	}
}

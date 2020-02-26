volatile long timer1 = micros();
volatile long timer2 = 0;

//volatile uint16_t count = 0;
uint32_t timer = micros();
uint32_t timerOlder = micros();
uint8_t flagRpm = 0;
//float Rpm=0;

#define NB_RPM 5

typedef struct
{
	uint16_t rpm;

	uint32_t timerOlder;
	uint32_t timer;

	uint16_t rpmTableau[NB_RPM];
	uint8_t	count;	

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
		timer2 = timer1;
		timer1 = micros();
}
void rpm_init(rpm_t * rpm)
{
	int i = 0;

	rpm->rpm = 0;
	rpm->timer = micros();
	rpm->timerOlder = micros();
	rpm->count = 0;
	for(i =0; i<NB_RPM; i++)
	{
	rpm->rpmTableau[i] = 0;
	}
}
void rpm_calcul(rpm_t * rpm)
{
	int i = 0;
	uint16_t tmpRpm = rpm->rpm;
	uint32_t sommeRpm = 0;
	uint32_t temps = timer1 - timer2; // Enregistre local car peut changer a tout moment
	//calcul 
	if(temps != 0) //eviter les division par 0
	{
		Serial.print("temps : ");
		Serial.println(temps);
	tmpRpm = (uint32_t)((1/((temps)*0.000001))*60);
	}
			
	// Si il n'y a aucun uptate de valeur depuir 0.6 seconde sois 100Hz RPM = 0
	if(micros() - timer2 > 600000)
	{
		tmpRpm = 0;
	}

	//Mettre la valeur dans le Tableau RPM 
	if(rpm->count == NB_RPM) //Remttre a 0 dans le count du tableau si on a attein le max
	{
		rpm->count = 0;
	}

	rpm->rpmTableau[rpm->count] = tmpRpm;
	rpm->count++;

	for (i = 0; i < NB_RPM;i++)
	{
		sommeRpm = sommeRpm + rpm->rpmTableau[i];
	}

	Serial.print("SommeRpm :");
	Serial.println(sommeRpm);
	rpm->rpm = sommeRpm/NB_RPM;

}



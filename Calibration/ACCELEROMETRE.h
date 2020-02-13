#pragma once



#define ACC_0 A0
#define ACC_1 A1
#define ACC_2 A2
#define ACC_3 A3
#define ACC_4 A4


#define GRAVITE 9.81
#define ZERO_REF_ACC_0 520
#define ECHELLE_ACC_0 107


/*====================================DEFINE====================================*/

















/*====================================TYPEDEF====================================*/

typedef struct
{
  unsigned int rawAcc;
  float gAcc;
  int count;
}T_ACC;


/*====================================Declaration====================================*/
void ACC_init(T_ACC*);
void ACC_init_tableau(T_ACC*,int dimension);
void ACC_convertRawToG(T_ACC* acc, float zero_ref, float echelle);
void ACC_read(T_ACC* acc, int analogPin, int count);
void ACC_afficher(T_ACC*acc);



/*====================================Definition====================================*/
//init
void ACC_init(T_ACC* acc)
{
  acc->rawAcc = 0;
  acc->gAcc = 0;
  acc->count = 0;
}
void ACC_init_tableau(T_ACC* acc,int dimension)
{
  int i;
   for(i=0;i<dimension;i++)
  {
    ACC_init(&acc[i]);
  }
}

//convert
void ACC_convertRawToG(T_ACC* acc, float zero_ref, float echelle)
{
  if(acc->rawAcc != 0)
  acc->gAcc = ((acc->rawAcc - zero_ref) / echelle)*GRAVITE;
  else
  acc->gAcc = 0;
}

//read
void ACC_read(T_ACC* acc, int analogPin, int count)
{
  acc->rawAcc = analogRead(analogPin);
  acc->count = count;
}

//afficher
void ACC_afficher(T_ACC*acc)
{
    Serial.print("\n");
    Serial.print(acc->count);
    Serial.print("\t");
    Serial.print(acc->rawAcc);
    Serial.print("\t");
    Serial.print(acc->gAcc);
   // Serial.print("\n");
}

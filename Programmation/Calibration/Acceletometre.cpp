
#include "Accelerometre.h"






/*Definition des fonctions  ===================================================*/
//config
void Acc_config()
{
  
}
//init
void ACC_init(T_ACC *acc)
{
  acc->rawAcc = 0;
  acc->gAcc = 0;
  acc->count = 0;
}
void ACC_init_tableau(T_ACC *acc, int dimension)
{
  int i;
  for (i = 0; i < dimension; i++)
  {
    ACC_init(&acc[i]);
  }
}

//convert
void ACC_convertRawToG(T_ACC *acc, float zero_ref, float echelle)
{
  if (acc->rawAcc != 0)
    acc->gAcc = ((acc->rawAcc - zero_ref) / echelle) * GRAVITE;
  else
    acc->gAcc = 0;
}

//read
void ACC_read(T_ACC *acc, int analogPin, int count)
{
  acc->rawAcc = analogRead(analogPin);
  acc->count = count;
}

void ACC_read_raw_acc(uint16_t *rawAcc, int analogPin, int count)
{
  *rawAcc = analogRead(analogPin);
  
}
//afficher
void ACC_afficher(T_ACC *acc)
{
  Serial.print("\n");
  Serial.print(acc->count);
  Serial.print("\t");
  Serial.print(acc->rawAcc);
  Serial.print("\t");
  Serial.print(acc->gAcc);
  // Serial.print("\n");
}

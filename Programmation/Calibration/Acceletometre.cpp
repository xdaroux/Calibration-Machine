#include "Accelerometre.h"
/*Declarations des fonctions Private============================================*/

/*Definition des fonctions  ===================================================*/
//config
void Acc_config_pin(void)
{
  pinMode(pin_ACC_0_CONNECTED, INPUT_PULLUP);
  pinMode(pin_ACC_1_CONNECTED, INPUT_PULLUP);
  pinMode(pin_ACC_2_CONNECTED, INPUT_PULLUP);
  pinMode(pin_ACC_3_CONNECTED, INPUT_PULLUP);
  pinMode(pin_ACC_4_CONNECTED, INPUT_PULLUP);
}
void Acc_config_init(T_Acc_config *accConfig)
{
  accConfig->numberAccConnected = 0;
  accConfig->testAccNum = 0;
  for (int i = 0; i < NOMBRE_ACC_MAX; i++)
  {
    accConfig->pinAnalogReadSequence[i] = 0;
    accConfig->pinAccConnected[i] = HIGH;
  }
}

//Acc_config_change
void Acc_config_change(T_Acc_config *accConfig)
{
  int8_t tmp = 0;
  int8_t i = 0;
  if (DEBUG)
  {
    Serial.println("INIT");
    delay(1000);
    Serial.print(digitalRead(pin_ACC_0_CONNECTED));
    Serial.print("  !=  ");
    Serial.println(accConfig->pinAccConnected[ACC_0]);
    Serial.print(digitalRead(pin_ACC_1_CONNECTED));
    Serial.print("  !=  ");
    Serial.println(accConfig->pinAccConnected[ACC_1]);
    Serial.print(digitalRead(pin_ACC_2_CONNECTED));
    Serial.print("  !=  ");
    Serial.println(accConfig->pinAccConnected[ACC_2]);
    Serial.print(digitalRead(pin_ACC_3_CONNECTED));
    Serial.print("  !=  ");
    Serial.println(accConfig->pinAccConnected[ACC_3]);
    Serial.print(digitalRead(pin_ACC_4_CONNECTED));
    Serial.print("  !=  ");
    Serial.println(accConfig->pinAccConnected[ACC_4]);
  }
  //Gestion nombre Acc connecter

  // si un de ses condition est vrai, on refait le mapping

  if (digitalRead(pin_ACC_0_CONNECTED) != accConfig->pinAccConnected[ACC_0])
  {
    if (DEBUG)
    {
      Serial.println("Etat Acc 0 Modifier");
    }
    tmp = -1;
  }
  if (digitalRead(pin_ACC_1_CONNECTED) != accConfig->pinAccConnected[ACC_1])
  {
    if (DEBUG)
    {
      Serial.println("Etat Acc 1 Modifier");
    }
    tmp = -1;
  }
  if (digitalRead(pin_ACC_2_CONNECTED) != accConfig->pinAccConnected[ACC_2])
  {
    if (DEBUG)
    {
      Serial.println("Etat Acc 2 Modifier");
    }
    tmp = -1;
  }
  if (digitalRead(pin_ACC_3_CONNECTED) != accConfig->pinAccConnected[ACC_3])
  {
    if (DEBUG)
    {
      Serial.println("Etat Acc 3 Modifier");
    }
    tmp = -1;
  }
  if (digitalRead(pin_ACC_4_CONNECTED) != accConfig->pinAccConnected[ACC_4])
  {
    if (DEBUG)
    {
      Serial.println("Etat Acc 4 Modifier");
    }
    tmp = -1;
  }

  //mapping all Accelerometer connected
  if (DEBUG)
  {
    Serial.print("tmp : ");
    Serial.println(tmp);
  }

  if (tmp == -1)
  {
    accConfig->numberAccConnected = 0;
    for(i = 0; i<NOMBRE_ACC_MAX;i++)
    {
       accConfig->pinAnalogReadSequence[i] = -1;
    }

    if (digitalRead(pin_ACC_0_CONNECTED) == LOW)
    {
      accConfig->pinAnalogReadSequence[accConfig->numberAccConnected] = pin_ACC_0;
      accConfig->numberAccConnected++;
      accConfig->pinAccConnected[ACC_0] = LOW;
    }
    else
    {
      accConfig->pinAccConnected[ACC_0] = HIGH;
    }

    if (digitalRead(pin_ACC_1_CONNECTED) == LOW)
    {
      accConfig->pinAnalogReadSequence[accConfig->numberAccConnected] = pin_ACC_1;
      accConfig->numberAccConnected++;
      accConfig->pinAccConnected[ACC_1] = LOW;
    }
    else
    {
      accConfig->pinAccConnected[ACC_1] = HIGH;
    }

    if (digitalRead(pin_ACC_2_CONNECTED) == LOW)
    {
      accConfig->pinAnalogReadSequence[accConfig->numberAccConnected] = pin_ACC_2;
      accConfig->numberAccConnected++;
      accConfig->pinAccConnected[ACC_2] = LOW;
    }
    else
    {
      accConfig->pinAccConnected[ACC_2] = HIGH;
    }

    if (digitalRead(pin_ACC_3_CONNECTED) == LOW)
    {
      accConfig->pinAnalogReadSequence[accConfig->numberAccConnected] = pin_ACC_3;
      accConfig->numberAccConnected++;
      accConfig->pinAccConnected[ACC_3] = LOW;
    }
    else
    {
      accConfig->pinAccConnected[ACC_3] = HIGH;
    }

    if (digitalRead(pin_ACC_4_CONNECTED) == LOW)
    {
      accConfig->pinAnalogReadSequence[accConfig->numberAccConnected] = pin_ACC_4;
      accConfig->numberAccConnected++;
      accConfig->pinAccConnected[ACC_4] = LOW;
    }
    else
    {
      accConfig->pinAccConnected[ACC_4] = HIGH;
    }
    if (DEBUG)
    {
      Serial.print("Number ACC Connected : ");
      Serial.println(accConfig->numberAccConnected);

      for (i = 0; i < NOMBRE_ACC_MAX; i++)
      {
        Serial.print("Valeur dans sequence de pin :");
        Serial.println(accConfig->pinAnalogReadSequence[i]);
      }
    }
    tmp = 0;
  } // fin mapping
}

//init
void ACC_init(T_ACC *acc)
{
  acc->rawAcc = 0;
  acc->gAcc = 0;
  acc->count = 0;
}

//ACC_init_taleau
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

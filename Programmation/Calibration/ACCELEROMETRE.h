#pragma once


#include <Arduino.h>



/*Pin Define  =================================================================*/
#define pin_ACC_0 A0
#define pin_ACC_1 A1
#define pin_ACC_2 A2
#define pin_ACC_3 A3
#define pin_ACC_4 A4

#define pin_ACC_0_CONNECTED  3
#define pin_ACC_1_CONNECTED  4
#define pin_ACC_2_CONNECTED  5
#define pin_ACC_3_CONNECTED  6
#define pin_ACC_4_CONNECTED  7

enum{ACC_0,ACC_1,ACC_2,ACC_3,ACC_4};
/*Define  =====================================================================*/
#define GRAVITE 9.81
#define ZERO_REF_ACC_0 520
#define ECHELLE_ACC_0 107

/*Typedef =====================================================================*/

typedef struct
{
  uint16_t rawAcc;
  float gAcc;
  int count;
} T_ACC;

/*Declaration des fonctions Public==============================================*/
void Acc_config(void);
void ACC_init(T_ACC *);
void ACC_init_tableau(T_ACC *, int dimension);
void ACC_convertRawToG(T_ACC *acc, float zero_ref, float echelle);
void ACC_read(T_ACC *acc, int analogPin, int count);
void ACC_read_raw_acc(uint16_t *rawAcc, int analogPin, int count);
void ACC_afficher(T_ACC *acc);


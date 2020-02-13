#define DIMENSION 180
float acc[DIMENSION];

// ici je veux juste contruire algo qui trouve les valeur peak de l'echantillon analyse !!   













void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float high;
  init_tableau(acc,DIMENSION);
  high = algo_peak(acc,DIMENSION);

  Serial.println(high);
  delay(1000);
}


void init_tableau(float* tableau, int dimension)
{
  int i;
  for(i=0;i<DIMENSION;i++)
  {
     // sa cetais justebpour mes test ,, // faut maintenant tout les t set a 0 !! 
    if(i<45)
    tableau[i] = i;
    else if(i<90 || i>135) 
    tableau[i] = tableau[i-1];
    else if(i<135)
    tableau[i] = tableau[i-1]-1;

    
    
    
  }
  tableau[40] = -1200;
}

float algo_peak(float*tableau, int dimension)

{
  int i;
  float highestValue = 0;
  float lowestValue = 0;

  for(i=0;i<DIMENSION;i++)
  {
    if(tableau[i] > highestValue)
      highestValue = tableau[i];

    if(tableau[i] < lowestValue)
    lowestValue = tableau[i]; 
  }
  return lowestValue;
}

#include<Wire.h> //This library allows you to communicate with I2C / TWI devices.
//As descrições são da documentação das bibliotecas no site do Arduíno

// Endereço que usado para o mestre se comunicar com este arduino
#define meuendereco 0x102

void setup()
{
  //Definindo pinos como saída
  pinMode(2, OUTPUT); //A
  pinMode(3, OUTPUT); //B
  pinMode(4, OUTPUT); //C
  pinMode(5, OUTPUT); //D
  pinMode(6, OUTPUT); //E
  pinMode(7, OUTPUT); //F
  pinMode(8, OUTPUT); //G
  pinMode(9, OUTPUT); //DP
  zero();
  Wire.begin(meuendereco);
  Wire.onReceive(receptor); 
}

void loop()
{ 
}

//troca de informação entre os arduino
void receptor (int quantidade)
{
  int recebido = Wire.read();
  if(recebido == 1) //se a info do mestre for 1, contar de 5 a 0 em 5 segundos.
  {
    contagem();
  }
}

void contagem()
{
  cinco();
  delay(1000);
  quatro();
  delay(1000);
  tres();
  delay(1000);
  dois();
  delay(1000);
  um();
  delay(1000);
  zero();
}

void cinco()
{ 
  //{ 1,0,1,1,0,1,1 } //DIGITO 5
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}

void quatro()
{ 
  //{ 0,1,1,0,0,1,1 },  //DIGITO 4
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}

void tres()
{ 
  //{ 1,1,1,1,0,0,1 },  //DIGITO 3
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
}

void dois()
{ 
  //{ 1,1,0,1,1,0,1 },  //DIGITO 2
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
}

void um()
{ 
  //{ 0,1,1,0,0,0,0 },  //DIGITO 1
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
}

void zero()
{ 
  //{ 1,1,1,1,1,1,0 },  //DIGITO 0
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
}


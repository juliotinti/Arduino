#include <LiquidCrystal.h> //This library allows an Arduino board to control LiquidCrystal displays (LCDs) based on the Hitachi HD44780 (or a compatible) chipset, which is found on most text-based LCDs.

//----------------------Display LCD---------------------------------//
//Pinos para o display 16x2
LiquidCrystal lcd(13, 12, 16, 17, 18, 19);
//----------------------Display LCD---------------------------------//

//------------------------------LED-----------------------------------//
int led = 11;
//------------------------------LED-----------------------------------//

//----------------------Sensor Temperatura----------------------------//
const int LM35 = A0; //Pino do LM35
int temperatura; //Temperatura medida
//----------------------Sensor Temperatura----------------------------//

int pedirFreq = 0;
int freq; //frequencia de leitura
int tecla;
//determinará se é para fazer leitura ou não, com base no *
int cond = 0;  //se impar vai fazer a leitura
float freqHz; //apenas para mostrar
float tempCelsius;
float tensao;

void setup() 
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(LM35, INPUT);
  pedirFrequencia();
  
  lcd.begin(16, 2);
  statusDisplay();
}

void loop()
{
  if (Serial.available() > 0) 
  {
    //leitura do teclado:
    tecla = Serial.read();

    //-------------------------Definição---------------------------------//
    //Só irá fazer algo se for umas das teclas pedidas no projeto
    //Vamos considerar os valores decimais da tabela ASCII:
    //R(82), r(114) e *(42)
    //-------------------------Definição---------------------------------//

    //reseta a frequencia pedida
    if(tecla == 82 || tecla == 114) //R(82) e r(114) 
    {
      pedirFrequencia();
    }
    else if(tecla == 42) //*(42)
    {
      analogWrite(led, 0);
      cond++; //se o incremento deixar cond impar vai ler, e se deixar par não vai ler
    }
  }
  //condição para leitura
  if(cond%2 != 0) //se impar vai ler
  {
    lerTemp();
  }
}

void pedirFrequencia()
{
  Serial.println(""); //separar as informações
  Serial.println("Frequencias possiveis (Digite o numero da esquerda para a frequencia desejada): ");
  Serial.println("(0) - 2.0hz [duas leituras por segundo]");
  Serial.println("(1) - 1.0hz [uma leitura por segundo");
  Serial.println("(2) - 0.5hz [uma leitura a cada dois segundos]");
  Serial.println("(3) - 0.2hz [uma leitura a cada cinco segundos]");
  Serial.println("(4) - 0.1hz [uma leitura a cada dez segundos]");
  pedirFreq = 0; 

  while(pedirFreq == 0)
  {
    tecla = Serial.read();
    if(tecla == 48) //0(48)
    {
      freq = 500; 
      freqHz = 2.0;
      pedirFreq = 1;
    }
    else if(tecla == 49) //1(49)
    {
      freq = 1000; 
      freqHz = 1.0;
      pedirFreq = 1;
    }
    else if(tecla == 50) //2(50)
    {
      freq = 2000; 
      freqHz = 0.5;
      pedirFreq = 1;
    }
    else if(tecla == 51) //3(51)
    {
      freq = 5000; 
      freqHz = 0.2;
      pedirFreq = 1;
    }
    else if(tecla == 52) //4(52)
    {
      freq = 10000;
      freqHz = 0.1; 
      pedirFreq = 1;
    }
  }
  Serial.println(""); //separar as informações
  Serial.print("Frequencia Escolhida: ");
  Serial.print(freqHz);
  Serial.println("hz");
}

void lerTemp()
{
  //leitura da temp 
  int leitura = analogRead(LM35); 
  float tensao = leitura * 5.0;
  tensao = tensao/1024.0;
  tempCelsius = (tensao - 0.5) * 100;
  Serial.println(tempCelsius); 
  statusDisplay();
  //Controlar intensidade da led com base na leitura
  controleLed(leitura);

  delay(freq); 
}



void controleLed(int leitura)
{
  if(leitura > 153) //153 é o valor da leitura quando o sensor tem 25 graus, se for acima de 25 se comporta como um dimmer
  {
    int pwm = map(leitura,153,358,0,255);
  	analogWrite(led, pwm);
  }
  else //se for menor que 25° fica desligado
  {
    analogWrite(led, 0);
  }
}

void statusDisplay()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Freq (hz): "); //Mostrar freq
  lcd.setCursor(11,0);
  lcd.print(freqHz);
  lcd.setCursor(14,0);
  lcd.setCursor(0,1);
  lcd.print("Leitura: ");
  lcd.setCursor(9,1);
  lcd.print(tempCelsius);
}


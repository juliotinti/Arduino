//----------------------Sensor Temperatura----------------------------//
const int LM35 = A0; //Pino do LM35
int arCond = 3;
int temperatura; //Temperatura medida
float tempCelsius;
float tensao;
int leitura;
float setTemp; //valor de temperatura setado pelo usuário
String tempPedido = String("");
String condTemp = String("Temperatura abaixo da setada");
int pedirTemp = 0;
//----------------------Sensor Temperatura----------------------------//

//----------------------LDR----------------------------//
const int LDR = A1; //Pino do LDR
int luzEntrada = 13;
int valorClaridade = 0;
String condEntrada = String("Luz da entrada apagada");
//----------------------LDR----------------------------//

//----------------------Prenseça Quarto----------------------------//
int sensorQuarto = 5;
int lampadaQuarto = 12; 
String condQuarto = String("Quarto vazio");
//----------------------Prenseça Quarto----------------------------//

//----------------------Presença Alarme----------------------------//
int sensorAlarme = 10;
int buzzerAlarme = 9; 
String condAlarme = String("Ambiente Seguro");
//----------------------Presença Alarme----------------------------//

//----------------------Sensor de Gás----------------------------//
const int sensorGas = A5; //Pino do LDR
int valorGas = 0;
int ledCozinha = 2; 
int ventiladorCozinha = 11; 
int buzzerCozinha = 8;
String condCozinha = String("Cozinha em situação segura");
//----------------------Sensor de Gás----------------------------//

int tecla; 
int cond = 0; 

void setup() 
{
  //Gás
  pinMode(LDR, INPUT);
  pinMode(ledCozinha, OUTPUT);
  pinMode(ventiladorCozinha, OUTPUT);
  pinMode(buzzerCozinha, OUTPUT);

  //Temperatura
  pinMode(LM35, INPUT);
  pinMode(arCond, OUTPUT);

  //LDR
  pinMode(LDR, INPUT);
  pinMode(luzEntrada, OUTPUT);

  //Quarto
  pinMode(sensorQuarto, INPUT);
  pinMode(lampadaQuarto, OUTPUT);

  //Alarme
  pinMode(sensorAlarme, INPUT);
  pinMode(buzzerAlarme, OUTPUT);

  Serial.begin(9600);
  pedirTemperatura();
}

void loop() 
{
  //leitura do teclado:
  tecla = Serial.read();

  //R(82), r(114) e *(42)

  //reseta o setTemp
  if(tecla == 82 || tecla == 114) //R(82) e r(114) 
  {
    pedirTemperatura();
  }
  else if(tecla == 42) //*(42)
  {
    cond++; //se o incremento deixar cond impar vai ler, e se deixar par não vai ler
  }
  if(cond%2 != 0) //se impar vai fazer as leituras
  {
    lerTemp();
    lerClaridade();
    lerGas();
    presencaQuarto();
    presencaAlarme();
    statusSerial();
    delay(50);
  }
}

void pedirTemperatura()
{
  Serial.println("Digite a temperatura desejada: "); 
  pedirTemp = 0; 

  while(pedirTemp == 0)
  {
    tempPedido = Serial.readString();
    Serial.println(tempPedido);
    setTemp = tempPedido.toFloat();
    if(tempPedido != 0)
    {
      pedirTemp = 1;
    }
  }
  Serial.println(""); //separar as informações
  Serial.print("Temperatura Escolhida: ");
  Serial.print(setTemp);
  Serial.println(" C");
  Serial.println("");
  Serial.println("Digite * para comecar");
}

void lerTemp()
{
  //leitura da temp 
  leitura = analogRead(LM35); 
  tensao = leitura * 5.0;
  tensao = tensao/1024.0;
  tempCelsius = (tensao - 0.5) * 100;
  if(tempCelsius > setTemp)
  {
    digitalWrite(arCond, HIGH);
    condTemp = "Temperatura acima da setada";
  }
  else
  {
    digitalWrite(arCond, LOW);
    condTemp = "Temperatura abaixo da setada";
  }
}

void lerClaridade()
{
  valorClaridade = analogRead(LDR);
  //varia entre 0 e 1023 o valor dado pelo sensor
  if(valorClaridade < 400) //se estiver escurecendo, no período da tarde, irá ligar a luz
  {
    digitalWrite(luzEntrada, HIGH);
    condEntrada = "Luz da entrada acesa";
  }
  else
  {
    digitalWrite(luzEntrada, LOW);
    condEntrada = "Luz da entrada apagada";
  }
}

void lerGas()
{
  valorGas = analogRead(sensorGas);
  if(valorGas > 250)
  {
    condCozinha = "Gas Vazando!!! EMERGENCIA";
    digitalWrite(ledCozinha, HIGH);
    digitalWrite(ventiladorCozinha, HIGH);
    tone(buzzerCozinha, 1000);
  }
  else
  {
    digitalWrite(ledCozinha, LOW);
    digitalWrite(ventiladorCozinha, LOW);
    noTone(buzzerCozinha);
    condCozinha = "Cozinha em situacao segura";
  }
}

void presencaQuarto()
{
  if(digitalRead(sensorQuarto) == HIGH)
  {
    digitalWrite(lampadaQuarto, HIGH);
    condQuarto = "Tem pessoas no quarto";
  }
  else 
  {
    digitalWrite(lampadaQuarto, LOW);
    condQuarto = "Quarto vazio";
  }
}

void presencaAlarme()
{
  if(digitalRead(sensorAlarme) == HIGH)
  {
    digitalWrite(buzzerAlarme, HIGH);
    condAlarme = "Alarme tocou!!! EMERGENCIA";
  }
  else 
  {
    digitalWrite(buzzerAlarme, LOW);
    condAlarme = "Ambiente Seguro";
  }
}

void statusSerial()
{
  Serial.println("------------------------------------------");
  Serial.print("Temperatura Setada: ");
  Serial.println(setTemp);
  Serial.print("Temperatura: ");
  Serial.print(tempCelsius);
  Serial.print(" - ");
  Serial.println(condTemp); 
  Serial.print("Entrada Casa: ");
  Serial.println(condEntrada); 
  Serial.print("Condicao Cozinha: ");
  Serial.println(condCozinha); 
  Serial.print("Condicao Quarto: ");
  Serial.println(condQuarto);
  Serial.print("Condicao Alarme: ");
  Serial.println(condAlarme); 
  Serial.println("------------------------------------------");
}









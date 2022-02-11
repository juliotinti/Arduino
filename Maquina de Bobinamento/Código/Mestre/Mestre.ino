#include <LiquidCrystal.h> //This library allows an Arduino board to control LiquidCrystal displays (LCDs) based on the Hitachi HD44780 (or a compatible) chipset, which is found on most text-based LCDs.

//----------------------Display LCD---------------------------------//
//Pinos para o display 16x2
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);
//----------------------Display LCD---------------------------------//

//-------------------------Motores---------------------------------//
#define pinMotorA_AH 5 //para o sentido anti-horário do motor A
#define pinMotorA_H 6 //para o sentido horário do motor A
#define pinEncoder_motorA 2
#define pinMotorB_AH 10 //para o sentido anti-horário do motor B
#define pinMotorB_H 9 //para o sentido horário do motor B
#define pinEncoder_motorB 12
float contadorVoltas = 0.0;
int movimento; //flag para saber se o motor está em movimento... 1 para movimento e 0 para parado 
String sentido = String(); //se for H é horário se for AH é anti-horário
int parar = 0; //para parar o movimento a partir do <espaço>
int tempo; //duracao do movimento do motor
//-------------------------Motores---------------------------------//

//-------------------------LED---------------------------------//
#define pinoLed 13
int tempoLed; //tempo que o LED irá piscar
//-------------------------LED---------------------------------//

int tecla;
float pwmMax = 255.0; //O valor máximo de PWM é 255
float rpmMax = 52.0; //RPM máximo do motor
float velocidade = 0.0; //a velocidade que o motor irá usar
float voltas;
int tempoVolta = 1*60/((velocidade/pwmMax)*rpmMax); //tempo de uma única volta
int voltasDadas = 0; //indica a qtd de voltas dadas durante o movimento
int pedirVolta = 0; //0 indica que não foi pedido ainda e 1 indica que será pedido 
String auxiliar = String();


void setup() 
{
  Serial.begin(9600);
  pinMode(pinoLed, OUTPUT);
  lcd.begin(16, 2);
  //Garantir que os motores estão parados
  desligarMotores();
  statusDisplay();
  voltasUsuario();
}

void loop()
{
  if (Serial.available() > 0) 
  {
    //leitura do teclado:
    tecla = Serial.read();

    //para conseguirmos ver o que foi digitado
    Serial.print("Foi recebido: ");
    Serial.println(tecla);

    //-------------------------Definição---------------------------------//
    //Só irá fazer algo se for umas das teclas pedidas no projeto
    //Vamos considerar os valores decimais da tabela ASCII:
    //↓(147) ↑(145) →(146) ←(144) <espaço>(32) q(113) Q(81) w(119) W(87) 0(48) 1(49) 2(50) 3(51) 4(52) 5(53) 6(54) 7(55) 8(56) 9(57)
    //-------------------------Definição---------------------------------//

    //diminui a velocidade de rotação
    if(tecla == 147) //↓(147)
    {
      diminuirVelocidade();
    }
    //aumenta a velocidade de rotação
    else if (tecla == 145) //↑(145)
    {
      aumentarVelocidade();
    }
    //sentido horário ou sentido anti-horário
    else if(tecla == 146 || tecla == 144) //→(146) ←(144)
    {
      sentidoMotores(tecla);
    }
    //parar o motor
    else if(tecla == 32) //<espaço>(32)
    {
      parar = 1;
    }
    //girar 90° ou girar 180°, só pode acontecer se o motor estiver parado. 
    else if((tecla == 113 || tecla == 81 || tecla == 119 || tecla == 87) && movimento == 0) //q(113) Q(81) w(119) W(87)
    {
      movimentoPrecisao(tecla);
    }
    //modifica a velocidade
    else if(tecla >=48 && tecla <=57 ) //0(48) 1(49) 2(50) 3(51) 4(52) 5(53) 6(54) 7(55) 8(56) 9(57)
    { 
      definirVelocidade(tecla);
    }
    //reseta a qtd de voltas
    else if(tecla == 82 || tecla == 114) //R(82) e r(114) adicionei essas para resetar o número de voltas pedidas
    {
      voltas = 0.0; 
      pedirVolta = 0;
      voltasUsuario();
    }
  }
}

void voltasUsuario()
{
  Serial.println("Digite a quantidade de voltas: ");
  voltas = 0.0;
  while(pedirVolta == 0)
  {
  	auxiliar = Serial.readString();
    if(auxiliar != "")
    {
  		pedirVolta = 1;
      
    }
  }
  voltas = auxiliar.toFloat();
  Serial.print("Quantidade de voltas escolhida: ");
  Serial.println(voltas);
}

void diminuirVelocidade()
{
  if(velocidade > pwmMax/10) //velocidade mínima = rpm_Max/10
  {
    velocidade = velocidade - pwmMax/10; //diminuir 10% da velocidade máxima 
    tempoLed = tempoLed - 50;
    statusDisplay();
  }
}

void aumentarVelocidade()
{
  if(velocidade < pwmMax) //velocidade máxima = rpm_Max
  {
    velocidade = velocidade + pwmMax/10; //aumentar 10% da velocidade máxima
    tempoLed = tempoLed + 50;
  	statusDisplay();
  }
}

void sentidoMotores(int tecla)
{
  tempo = voltas*60/((velocidade/pwmMax)*rpmMax);
  movimento = 1;
  voltasDadas = 0; //reseta a qtd de voltas dadas

  if(tecla == 146) //O motor A no sentido horário
  {
    //Como o motor A está no sentido horário, o motor B estará no anti-horário
    sentido = "H";
    statusDisplay(); //atualizar o sentido
    while (voltasDadas < voltas)
    {
      analogWrite(pinMotorA_AH, 0);
      analogWrite(pinMotorA_H, velocidade);
      analogWrite(pinMotorB_AH, velocidade);
      analogWrite(pinMotorB_H, 0);
      piscarLed();
      delay(tempoVolta*1000);  
      if(parar == 1)
      {
        break;
      }
      voltasDadas++;
      contadorVoltas++; 
      statusDisplay();   
    }
    desligarMotores(); 
  }
  else if(tecla == 144) //O motor A no sentido anti-horário
  {
    //Como o motor A está no sentido anti-horário, o motor B estará no horário
    sentido = "AH"; 
    statusDisplay(); //atualizar o sentido
    while (voltasDadas < voltas)
    {
      analogWrite(pinMotorA_AH, velocidade);
      analogWrite(pinMotorA_H, 0);
      analogWrite(pinMotorB_AH, 0);
      analogWrite(pinMotorB_H, velocidade);
      piscarLed();
      delay(tempoVolta*1000);  
      if(parar == 1)
      {
        break;
      }
      voltasDadas++;
      contadorVoltas++; 
      statusDisplay();   
    }
    desligarMotores();
  }
}

void desligarMotores()
{
  movimento = 0;
  analogWrite(pinMotorA_AH, 0);
  analogWrite(pinMotorA_H, 0);
  analogWrite(pinMotorB_AH, 0);
  analogWrite(pinMotorB_H, 0);
  desligarLed();
  parar = 0;
}

void movimentoPrecisao(int tecla) 
{
  //o movimento precisão tera a maior velocidade possível
  velocidade = pwmMax; 
  tempoLed = 100;
  statusDisplay();

  if(tecla == 113 || tecla == 81) //q(113) Q(81)
  {
    voltas = 0.25; // 1/4 de volta
    tempo = voltas*60/((velocidade/pwmMax)*rpmMax);
    movimento = 1;

    if(sentido = "AH")
    {
      analogWrite(pinMotorA_AH, velocidade);
      analogWrite(pinMotorA_H, 0);
      analogWrite(pinMotorB_AH, 0);
      analogWrite(pinMotorB_H, velocidade);
      piscarLed();
      delay(tempo*1000);
      desligarMotores();
      contadorVoltas = contadorVoltas + voltas;  
      statusDisplay();
    }
    else
    {
      analogWrite(pinMotorA_AH, 0);
      analogWrite(pinMotorA_H, velocidade);
      analogWrite(pinMotorB_AH, velocidade);
      analogWrite(pinMotorB_H, 0);
      piscarLed();
      delay(tempo*1000);
      desligarMotores();
      contadorVoltas = contadorVoltas + voltas; 
      statusDisplay();
    }
  }
  else //w(119) W(87)
  {
    voltas = 0.5; // 1/2 de volta
    tempo = voltas*60/((velocidade/pwmMax)*rpmMax);
    movimento = 1;
    if(sentido = "AH")
    {
      analogWrite(pinMotorA_AH, velocidade);
      analogWrite(pinMotorA_H, 0);
      analogWrite(pinMotorB_AH, 0);
      analogWrite(pinMotorB_H, velocidade);
      piscarLed();
      delay(tempo*1000);
      desligarMotores();
      contadorVoltas = contadorVoltas + voltas;  
      statusDisplay();
    }
    else
    {
      analogWrite(pinMotorA_AH, 0);
      analogWrite(pinMotorA_H, velocidade);
      analogWrite(pinMotorB_AH, velocidade);
      analogWrite(pinMotorB_H, 0);
      piscarLed();
      delay(tempo*1000);
      desligarMotores();
      contadorVoltas = contadorVoltas + voltas; 
      statusDisplay();
    }
  }
}

//Quando o usuário apertar de 0 a 9; 
void definirVelocidade(int valorDec)
{
  float valor = valorDec - 48.0;   //transição do ASCII para o número escolhido
  tempoLed = 600 - (valor*50);
  if(valor == 0.0) //se for 0 a velocidade é máxima
  {
    valor = 10.0;
    tempoLed = 100; //velocidade mais rápida da LED 
  }
  velocidade = pwmMax*(valor/10.0); 
  statusDisplay();
}

void statusDisplay()
{
  float rpm = (velocidade/pwmMax)*rpmMax;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("RPM: "); //Mostrar o RPM
  lcd.setCursor(5,0);
  lcd.print(rpm);
  lcd.setCursor(14,0);
  lcd.print(sentido); //mostrar o sentido de giro
  lcd.setCursor(0,1);
  lcd.print("Voltas: ");
  lcd.setCursor(8,1);
  lcd.print(contadorVoltas); //mostrar a quantidade de voltas
}


void piscarLed()
{
  for(int i = 0;i<10;i++)
  {
    digitalWrite(pinoLed, HIGH);
    delay(tempoLed);
    digitalWrite(pinoLed, LOW);
    delay(tempoLed);
  }
}

void desligarLed()
{
    digitalWrite(pinoLed, LOW);
}

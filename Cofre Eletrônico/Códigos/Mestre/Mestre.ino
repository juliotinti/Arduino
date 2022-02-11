#include<Wire.h> //This library allows you to communicate with I2C / TWI devices.
#include<Keypad.h> //Keypad is a library for using matrix style keypads with the Arduino. As of version 3.0 it now supports mulitple keypresses.
#include<Servo.h> //Allows Arduino boards to control a variety of servo motors.
//As descrições são da documentação das bibliotecas no site do Arduíno

//Endereço dos dois servos
#define enderecoServo1 0x101 //display
#define enderecoServo2 0x102 //sete segmentos

//senha
String senha = String("1234");
String tentativa = String(""); //tentativa da senha

//----------------------Buzzer---------------------------------//
#define buzzer 13
//----------------------Buzzer---------------------------------//

//----------------------Servo Motor----------------------------//
#define servoPin 12
Servo servo;
//----------------------Servo Motor----------------------------//

//----------------------Teclado Matricial----------------------//
//linhas e colunas do teclado matricial
const byte linhas = 4; 
const byte colunas = 4;

//matriz do teclado
char matrizTeclas[linhas][colunas] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

//Pinos ligados aos pinos 1, 2, 3 e 4 do teclado - Linhas
byte pinLinhas[linhas] = {3,4,5,6};
//Pinos ligados aos pinos 5, 6, 7 e 8 do teclado - Colunas
byte pinColunas[colunas] = {8,9,10,11};
Keypad teclado = Keypad(makeKeymap(matrizTeclas), pinLinhas, pinColunas, linhas, colunas);
//----------------------Teclado Matricial----------------------//

void setup()
{

  servo.attach(servoPin);
  servo.write(0); //iniciar no ângulo zero (posição incial da maçaneta)

  pinMode(buzzer, OUTPUT);
  Wire.begin();
  
  teclado.setDebounceTime(10); //tempo de atraso para leitura das teclas

  Serial.begin(9600);
}
 
void loop()
{
  char key = teclado.getKey(); 
  if(key != NO_KEY)
  {
    Serial.print("Digitado: ");
    Serial.println(key);
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    switch (key) 
    {
      case '#': senhaCheck();  //função para checar senha. Está mais embaixo no programa
        break;
      case '*': zerarTentativa(); //zerar a string da tentativa
        break;
      case 'A': mudarSenha(); //função para mudar senha. Está mais embaixo no programa
        break;
      case 'B': confirmarMudanca(); //função para confirmar nova senha. Está mais embaixo no programa
        break;
      case 'C': caracterInvalido(); //"Valor Inválido"
        break;
      case 'D': caracterInvalido(); //"Valor Inválido"
        break;
      default:
        tentativa.concat(key); //montar a senha
        enviarMensagem_Display(2); //mandar o "*" para o display
      	break;
    }
  }
}

//Ao pressionar #, verificar a senha
void senhaCheck()
{
  enviarMensagem_Display(4); //"Verificando..."
  delay(1000);
  if(tentativa == senha) 
  {
    enviarMensagem_Display(5); //"Aberto!!"
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
    //fazer a abertura da maçaneta com uma velocidade um pouco mais controlada 
    for(int angulo = 0; angulo < 180; angulo++)
    {
      servo.write(angulo);
      delay(15);
    }
    enviarMensagem_SeteSeg(1);
    delay(5000);
    //fechar a maçaneta
    for(int angulo = 180; angulo > 0; angulo--)
    {
      servo.write(angulo);
      delay(15);
    }
    tentativa = ""; 
    enviarMensagem_Display(1); //"Digite sua senha"
  }
  else 
  {
    enviarMensagem_Display(6); //"SENHA INVÁLIDA!!!"
    digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(buzzer, LOW);
    tentativa = "";
    enviarMensagem_Display(9); //"!!!Inativo!!!"
    delay(3000); //inatividade do sistema
    enviarMensagem_Display(1); //"Digite sua senha"
  }
}

void zerarTentativa()
{
  tentativa = "";
  enviarMensagem_Display(8);
}

void caracterInvalido()
{
  enviarMensagem_Display(3);
  delay(1000);
  tentativa = ""; //se digitar um caracter invalido, a tentativa será zerada
  enviarMensagem_Display(1);    
}

void mudarSenha()
{
  tentativa = "";//garantir que não tem nada escrito
  enviarMensagem_Display(7); //"Nova senha:"
}

void confirmarMudanca()
{
  senha = tentativa;
  zerarTentativa();
  enviarMensagem_Display(1);
}

void enviarMensagem_Display(int cond)
{
  Wire.beginTransmission(enderecoServo1);
  Wire.write(cond);
  Wire.endTransmission();
}

void enviarMensagem_SeteSeg(int cond)
{
  Wire.beginTransmission(enderecoServo2);
  Wire.write(cond);
  Wire.endTransmission();
}

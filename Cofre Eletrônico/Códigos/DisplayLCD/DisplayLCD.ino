#include<Wire.h> //This library allows you to communicate with I2C / TWI devices.
#include <LiquidCrystal.h> //This library allows an Arduino board to control LiquidCrystal displays (LCDs) based on the Hitachi HD44780 (or a compatible) chipset, which is found on most text-based LCDs.
//As descrições são da documentação das bibliotecas no site do Arduíno

//----------------------Display LCD---------------------------------//
//Pinos para o display 16x2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//----------------------Display LCD---------------------------------//

// Endereço usado para o mestre se comunicar com este arduino
#define meuendereco 0x101 

String mensagem = String(""); 

void setup() 
{
  Wire.begin(meuendereco);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite sua senha");
  Wire.onReceive(receptor);
}

void loop()
{
}

void receptor (int quantidade)
{
  int recebido = Wire.read();
  mensagens(recebido);
}  

void mensagens(int cond)
{
  if(cond == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print("Digite sua senha");
  }
  else if(cond == 2)
  {
    mensagem.concat("*");
    lcd.setCursor(0, 1);
    lcd.print(mensagem);
  }
  else if(cond == 3)
  {
    mensagem = "";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Valor Invalido  ");    
  }
  else if(cond == 4)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Verificando...");
    mensagem = "";    
  }
  else if(cond == 5)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aberto!!"); 
  }
  else if(cond == 6)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SENHA INVALIDA");  
  }
  else if(cond == 7)
  {
    mensagem = "";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nova senha:");    
  }
  else if(cond == 8)
  {
    mensagem = "                ";  
    lcd.setCursor(0, 1);
    lcd.print(mensagem);
    mensagem = "";
  }
  else if(cond == 9)
  { 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!!!Inativo!!!");
  } 
}


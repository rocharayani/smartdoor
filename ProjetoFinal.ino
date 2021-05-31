/* Inclusão de Bibliotecas */
#include <SPI.h>                
#include <MFRC522.h>            
#include <Servo.h>              
#include <Wire.h>               
#include <LiquidCrystal_I2C.h>
 
/* Definição de Pinos */

//Pinos do Sensor MFRC522 
#define SS_PIN 53
#define RST_PIN 6

//Pinos do Servo
#define pinServo 9

/* Pre definiçoes do sistema */

// Criação da insteancia MFRC522.
MFRC522 mfrc522(SS_PIN, RST_PIN);   

// Configuração da I2C e demais informaçoes para o LCD.
LiquidCrystal_I2C lcd(0x3f,2,1,0,4,5,6,7,3, POSITIVE); 

// Configuração do Servo Motor 
Servo servo1;
int grau = 0;

char st[20]; 
 
void setup()
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();          // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  
  Serial.println("Aproxime o seu cartão do leitor...");
  Serial.println();
  
  lcd.begin (16,2);       //Define o número de colunas e linhas do LCD: 
  lcd.setBacklight(HIGH); //LIGA O BACKLIGHT (LUZ DE FUNDO)
  
  mensageminicial();
  
  // Configuração inicial servo motor
  servo1.attach(pinServo);
  Serial.begin(9600);
  servo1.write(0);
  delay(2000);
}
 
void loop()
{
  // Leitura do chaveiros
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
      return;
  }
  // Leitura do cartão
  if (! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  
  //Mostra UID na serial (Bom para pegar o endereço do sensor)
  Serial.print("UID da tag :");
  String conteudo= "";
  
  //Retorna a mensagem com o ID do cartão ou chaveiro.
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  
  //UID 1 - Chaveiro
  if (conteudo.substring(1) == "19 E2 7F A3") 
  {
    Serial.println("Ola Usuário !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola Usuário !");
    lcd.setCursor(0,1);
    lcd.print("Acesso liberado!");
    servo1.write(180);
    delay(15000); // 15 segundos até o fechamento automatico
    servo1.write(0);
  }
  else
  {
    Serial.println("Acesso Negado!");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Acesso Negado!");
    lcd.setCursor(0,1);
    lcd.print("Favor tentar outro cartão!");
    delay(3000); 
  }
  mensageminicial();  
}

 
void mensageminicial()
{
  lcd.clear();
  lcd.print(" Aproxime o seu"); 
  lcd.setCursor(0,1);
  lcd.print("cartao do leitor"); 
}

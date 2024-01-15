/*
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

float BeepCurrentAmount = 120; // Peso
const float Total_Fair = 18;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo door;

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();

  door.attach(6);
  door.write(10); //closed

  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  lcd.setCursor(0, 0);
  lcd.print("Swipe Card");
}

void loop() {
  // put your main code here, to run repeatedly:
    

    if ( ! mfrc522.PICC_IsNewCardPresent()) 
      return;
    
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) 
      return;
    
    //Show UID on serial monitor
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    
    content.toUpperCase();
    if (content.substring(1) == "83 0A 9F FC" )  //change here the UID of the card/cards that you want to give access
    {
      BeepCurrentAmount -= Total_Fair;
      char amount[255];
      sprintf(amount, " %f", (double)BeepCurrentAmount);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Current Amount:");
      lcd.setCursor(0, 1);
      lcd.print(amount);
      
      door.write(90); //Open
      delay(3000);
      door.write(10); //close
    }
}
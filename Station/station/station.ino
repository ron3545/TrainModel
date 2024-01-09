#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

float BeepCurrentAmount = 120; // Peso
const float Total_Fair = 18;

const uint8_t ss_pin = 10;
const uint8_t rst_pin = 9;
const uint8_t BeepCardLoader_Button = 13;
const uint8_t rst = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

unsigned long BeepCardPrevTime = millis();
long CardTimeInterval = 1000;

unsigned long CardLoaderPrevTime = millis();
long LoaderTimeInterval = 2000;

LiquidCrystal lcd(rst, en, d4, d5, d6, d7);
MFRC522 mfrc522(ss_pin, rst_pin);
Servo door;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  SPI.begin();
  mfrc522.PCD_Init();

  door.attach(8);
  door.write(10); //closed

  pinMode(BeepCardLoader_Button, INPUT)
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long current_time = millis();

  if(current_time - BeepCardPrevTime > CardTimeInterval)
  {
    BeepCardPrevTime = current_time;
    if(!mfrc522.PICC_IsNewCardPresent() || mfrc522.PICC_ReadSCardSerial())
      return;

    String tagID = "";
    byte letter;
    for(byte i = 0; i < mfrc522.uid.size; i++)
    {
      tagID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    tagID.toUpperCase();

    if (tagID.substring(1) == "BD 31 15 2B" && BeepCurrentAmount > Total_Fair)
    {
      BeepCurrentAmount -= Total_Fair;
      char message[255];
      sprintf(message, "Current Amount: %f \n Total Fair: %f", BeepCurrentAmount, Total_Fair);
      lcd.print(message);
      
      door.write(90); //Open
      delay(3000);
      door.write(10); //close
    }
  }

  // For adding load on the master card
  if(current_time - CardLoaderPrevTime > LoaderTimeInterval)
  {
    CardLoaderPrevTime = current_time;
    bool buttonState = digitalRead(BeepCardLoader_Button);
    if(buttonState)
      BeepCurrentAmount += 10;
  }
}

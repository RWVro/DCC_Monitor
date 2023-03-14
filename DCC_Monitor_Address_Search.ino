
//====================================== Monitor Loco address =====================================
//
//
//=============================  Board = ESP32 Dev Module (NodeMCU ESP32) =========================

#define ONBOARD_LED 2
#define RPWM 19                 // to RPWM MotorShield pin 1 & 4
#define LPWM 18                 // to LPWM MotorShield pin 2 & 3

#include "Convert.h"

//=================================  VARIABLES =================================

int railComRX = 16;          // GPIO16 Connected to RailCom Detector RX
HardwareSerial Poort2(2);   // Define a Serial port instance called 'Poort2' using serial port 2

int receivedChar;
bool addressFlg = false;
String addressStr;

//================================= Setup =======================================

void PrintFileName()
{
  Serial.println("");
  Serial.println(__FILE__" ("__DATE__") "__TIME__);
  Serial.println("");
}

void setup()
{
  Serial.begin(500000);
  delay(1000);
  PrintFileName();

  // sets the digital pins as output or input
  //=========================================

  pinMode(ONBOARD_LED, OUTPUT);           // For test Blue Led
  digitalWrite(ONBOARD_LED, HIGH);

  pinMode(railComRX, INPUT);              // GPIO16 Connected to RailCom Detector RX

  Serial.println("Monitor started!");
  delay(1000);

  pinMode(ONBOARD_LED, OUTPUT);           // For test Blue Led
  digitalWrite(ONBOARD_LED, LOW);

  Poort2.begin(250000, SERIAL_8N1, railComRX, 17);          // Define and start serial port
  delay(1000);
}

//======================================= Main Program ==============================

void loop()
{
  while (Poort2.available())
  {
    inByte = Poort2.read();                             // Read byte
    if (inByte == 0)                                    // Skipp all 0 chars
    {
      return;
    }
    check_4_8Code();

    if (!test_4_8Code)                                   // Check if 4-8 code is ok, if ok , print
    {
      return;
    }
    convert4_8ToDec();

    if (addressFlg)
    {
      receivedChar = inByte;
      addressStr = "Loco Address=" + String (receivedChar);
      Serial.println(addressStr);
      addressFlg = false;
      receivedChar = 0;
    }

    if (test_4_8Decimal)                               // If valid decimal byte
    {
      if (inByte == 8)                                // Find decimal 8
      {
        addressFlg = true;
      }
    }
  }
}

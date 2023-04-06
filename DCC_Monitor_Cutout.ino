
//=============================== Monitor DCC Cutouts Program ===========================
//
//
//=========================  Board = ESP32 Dev Module (NodeMCU ESP32) ===================

#define ONBOARD_LED 2

#define railComRX 16
#define railComTX 17
#define railComInt 15

#include "Convert.h"

//======================================== Setup =======================================

void PrintFileName()
{
  Serial.println("");
  Serial.println("");
  Serial.println(__FILE__" ("__DATE__") "__TIME__);
  Serial.println("");
}

void setup()
{
  Serial.begin(500000);
  delay(1000);
  PrintFileName();

  pinMode(ONBOARD_LED, OUTPUT);           // For test Blue Led
  digitalWrite(ONBOARD_LED, HIGH);

  pinMode(railComInt, INPUT);             // GPIO15 input pin  int Raicom Detector

  delay(1000);
  Serial.println("Monitor Cutout Started!\n");

  pinMode(ONBOARD_LED, OUTPUT);           // For test   Blue Led
  digitalWrite(ONBOARD_LED, LOW);

  delay(3000);
  Poort2.begin(250000, SERIAL_8N1, railComRX, railComTX);          // Define and start serial port

  attachInterrupt((railComInt), GPIO15ToLow, FALLING);

  ClearrxFirstArray();
  ClearrxSecondArray();
}

//==================================================================================

void TestRailComInt()
{
  if (digitalRead(railComInt) == HIGH)
  {
    cutoutCompl = true;
  }
  else
  {
    cutoutCompl = false;
  }
}

void GoCutout1()
{
  if (rxFirstArray[0] == 4 | rxFirstArray[0] == 6)
  {
    if (rxFirstArrayCnt != 0)
    {
      //printFirstArray();
      cutoutCnt = 2;
    }
  }
  rxFirstArrayCnt = 0;
  cutoutStart = false;
  attachInterrupt((railComInt), GPIO15ToLow, FALLING);
}

void GoCutout2()
{
  if (rxFirstArray[0] == 4 | rxFirstArray[0] == 6)
  {
    if (rxSecondArrayCnt != 0)
    {
      //printSecondArray();
    }
  }
  SearchAddressCode();
  cutoutCnt = 1;
  ClearrxFirstArray();
  rxFirstArrayCnt = 0;
  ClearrxSecondArray();
  rxSecondArrayCnt = 0;
  cutoutStart = false;
  attachInterrupt((railComInt), GPIO15ToLow, FALLING);
}

//======================================= Main Program ==============================

void loop()
{
  if (cutoutStart)
  {
    while (Poort2.available())
    {
      inByte = Poort2.read();             // Read byte
      check_4_8Code();                    // Test if is 4_8Code

      if (test_4_8Code)                   // If 4_8Code is ok
      {
        convert4_8ToDec();                // Convert to decimal

        if (test_4_8Decimal)
        {
          if (cutoutCnt == 1)
          {
            rxFirstArray[rxFirstArrayCnt] = inByte;           // Byte into rxFirstArray
            rxFirstArrayCnt ++;
          }
          if (cutoutCnt == 2)
          {
            rxSecondArray[rxSecondArrayCnt] = inByte;         // Byte into rxSecondArray
            rxSecondArrayCnt ++;
          }
        }
      }
    }

    TestRailComInt();

    if (cutoutCompl && cutoutCnt == 1)                        // Cutout1 stop
    {
      GoCutout1();
      return;
    }

    if (cutoutCompl && cutoutCnt == 2)                      // Cutout2 stop
    {
      GoCutout2();
    }
  }
}

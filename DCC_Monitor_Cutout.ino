
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

//======================================= Main Program ==============================

void loop()
{
  if (cutoutStart)
  {
    if (cutoutCnt == 1)
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
            if (!findStartByteCO1)
            {
              if (inByte == 4 | inByte == 6)                  // Find decimal 4 or 6
              {
                findStartByteCO1 = true;
              }
            }
            if (findStartByteCO1)                             // Start saving bytes if rxFirstArray
            {
              rxFirstArray[rxFirstArrayCnt] = inByte;         // Byte into rxFirstArray
              rxFirstArrayCnt ++;
            }
          }
        }
      }
    }

    TestRailComInt();

    if (cutoutCnt == 2)
    {
      while (Poort2.available())
      {
        inByte = Poort2.read();                             // Read byte
        check_4_8Code();                                    // Test if is 4_8Code

        if (test_4_8Code)                                   // If 4_8Code is ok
        {
          convert4_8ToDec();                                // Convet to decimal
          if (test_4_8Decimal)
          {
            rxSecondArray[rxSecondArrayCnt] = inByte;      // Byte into rxSecondArray
            rxSecondArrayCnt ++;
          }
        }
      }
    }

    if (cutoutCompl)                                      // Cutout stop
    {
      if (findStartByteCO1)                               // Startbyte and first cutout find
      {
        cutoutCnt = 2;
      }
      if (!findStartByteCO1 && cutoutCnt == 2)
      {
        //printFirstArray();
        //printSecondArray();
        SearchAddressCode();
        cutoutCnt = 1;
        ClearrxFirstArray();
        rxFirstArrayCnt = 0;
        ClearrxSecondArray();
        rxSecondArrayCnt = 0;
      }
      findStartByteCO1 = false;
      cutoutStart = false;
      attachInterrupt((railComInt), GPIO15ToLow, FALLING);
    }
  }
}


//====================================== Monitor DCC Cutouts Program ==================================
//
//
//=================================  Board = ESP32 Dev Module (NodeMCU ESP32) =========================

#define ONBOARD_LED 2

#define railComRX 16
#define railComTX 17
#define railComInt 15

//#include "Convert.h"

hw_timer_t *CutoutTimer = NULL;
bool cutoutCompl = false;
int cutoutCnt = 1;;


//=================================== Timer Cutout ====================================

void IRAM_ATTR TestCutout()                                         // Cutout end
{
  cutoutCompl = true;
  Serial.println("Timer OK");
}

void IniCutoutTimer()                                               // Timer
{
  CutoutTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(CutoutTimer, &TestCutout, true);             // ISR = TestCutout
  timerAlarmWrite(CutoutTimer, 5000000, true);                      // 5 sec. (true = repeat)
  timerAlarmEnable(CutoutTimer);                                    //Just Enable

  cutoutCompl = false;
}


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

  //pinMode(railComInt, INPUT);             // GPIO15 input pin  int Raicom Detector

  delay(1000);
  Serial.println("Monitor started!");

  pinMode(ONBOARD_LED, OUTPUT);           // For test   Blue Led
  digitalWrite(ONBOARD_LED, LOW);

  delay(3000);
  
  IniCutoutTimer();
  Serial.println("Timer test started!");
}

//======================================= Main Program ==============================

void loop()
{
  
/*  
  if (intStart)
  {
    while (!cutoutCompl)                      // Cutout start
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
                rxFirstArray[rxFirstArrayCnt] = inByte;         // Byte into  array
                rxFirstArrayCnt ++;
              }
            }
          }
        }
      }

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
              rxSecondArray[rxSecondArrayCnt] = inByte;      // Byte into  rxSecondArray
              rxSecondArrayCnt ++;
            }
          }
        }
      }
    }

    if (cutoutCompl)                          // Cutout stop
    {
      if (findStartByteCO1)                  // Startbyte and first cutout find
      {
        cutoutCnt = 2;
      }
      if (!findStartByteCO1 && cutoutCnt == 2)
      {
        SearchAddressCode();
        cutoutCnt = 1;
        ClearrxFirstArray();
        rxFirstArrayCnt = 0;
        ClearrxSecondArray();
        rxSecondArrayCnt = 0;
      }
      findStartByteCO1 = false;
      intStart = false;
      attachInterrupt((railComInt), GPIO15ToLow, FALLING);
    }
  }
  */
}

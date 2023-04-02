
//====================================== Monitor DCC Cutouts Program ==================================
//
//
//=================================  Board = ESP32 Dev Module (NodeMCU ESP32) =========================

#define ONBOARD_LED 2

#define railComRX 16
#define railComTX 17
#define railComInt 15

bool intStart = false;
bool intStop = false;

//===================================== ISR GPIO15 ================

void IRAM_ATTR GPIO15ToLow()
{
  detachInterrupt(digitalPinToInterrupt(railComInt));
    
  Serial.println("F");
  intStart = true;
  intStop = false;
}

void IRAM_ATTR GPIO15ToHigh()
{
  detachInterrupt(digitalPinToInterrupt(railComInt));
  
  Serial.println("R");
  intStop = true;
  intStart = false;
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

  pinMode(railComInt, INPUT);             // GPIO15 input pin  int Raicom Detector

  delay(1000);
  Serial.println("Test interrupt started!");

  pinMode(ONBOARD_LED, OUTPUT);           // For test   Blue Led
  digitalWrite(ONBOARD_LED, LOW);

  delay(3000);

  attachInterrupt((railComInt), GPIO15ToLow, FALLING);
}


//======================================= Main Program ==============================

void loop()
{
  if (intStart)
  {
    attachInterrupt((railComInt), GPIO15ToHigh, RISING);
  }
  if (intStop)
  {
    attachInterrupt((railComInt), GPIO15ToLow, FALLING);
  }
}

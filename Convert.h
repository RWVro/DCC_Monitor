
HardwareSerial Poort2(2);   // Define a Serial port instance called 'Poort2' using serial port 2

hw_timer_t *CutoutTimer = NULL;
bool cutoutCompl = false;
int cutoutCnt = 1;;

int receiveCnt = 0;
int inByte = 0;

const int rxFirstArrayMax = 13;
int rxFirstArray[rxFirstArrayMax];    // [13] = char's
int rxFirstArrayCnt = 0;

const int rxSecondArrayMax = 13;
int rxSecondArray[rxSecondArrayMax];    // [13] = char's
int rxSecondArrayCnt = 0;

bool findStartByteCO1 = false;
int addressInt;

bool test_4_8Code = true;
bool test_4_8Decimal = true;

bool intStart = false;

//=================================== Timer Cutout ====================================

void IRAM_ATTR TestCutout()                                         // Cutout end
{
  cutoutCompl = true;
}

void IniCutoutTimer()                                               // Timer
{
  CutoutTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(CutoutTimer, &TestCutout, true);             // ISR = TestCutout
  timerAlarmWrite(CutoutTimer, 470, false);                         // 470 microsec. (true = repeat)
  timerAlarmEnable(CutoutTimer);                                    //Just Enable

  cutoutCompl = false;
}

//============================== 4-8 Table conversion ================================

int decodeArray[68] = {172, 170, 169, 165, 163, 166, 156, 154, 153, 149, 147, 150, 142, 141, 139, 177, 178, 180, 184, 116,
                       114, 108, 106, 105, 101, 99, 102, 92, 90, 89, 85, 83, 86, 78, 77, 75, 71, 113, 232, 228, 226, 209, 201,
                       197, 216, 212, 210, 202, 198, 204, 120, 23, 27, 29, 30, 46, 54, 58, 39, 43, 45, 53, 57, 51, 15, 240, 225, 31
                      };       // 31 is end of table (0001 1111)


int convertArray[67] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
                        27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
                        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66
                       };

//============================== Convert 4_8 Table to dec. ============================

void check_4_8Code()
{
  test_4_8Code = true;

  int compareValue = inByte;
  int i = 0;
  while (compareValue != decodeArray[i])
  {
    if (decodeArray[i] == 31)                  // End of table reached
    {
      test_4_8Code = false;
      return;
    }
    i++;
  }
}

void convert4_8ToDec()
{
  test_4_8Decimal = true;

  int compareValue = inByte;
  int i = 0;
  while (compareValue != decodeArray[i])
  {
    if (decodeArray[i] == 31)                  // End of table reached
    {
      test_4_8Decimal = false;
      return;
    }
    i++;
  }
  inByte = convertArray[i];
}

//======================================= Clear array ===================================

void ClearrxFirstArray()
{
  int i = 0;
  while (i < rxFirstArrayMax - 1)
  {
    rxFirstArray[i] = 0;
    i ++;
  }
}

void ClearrxSecondArray()
{
  int i = 0;
  while (i < rxSecondArrayMax - 1)
  {
    rxSecondArray[i] = 0;
    i ++;
  }
}

//==================================== Find Address Code ===================================

void DetectAddressCode()
{
  addressInt = 0;

  if (rxFirstArray[0] == 4 && rxFirstArray[1] == 0 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    return;
  }
  if (rxFirstArray[0] == 4 && rxFirstArray[1] == 0 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 64;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 0 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 128;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 0 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 192;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 1 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 256;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 1 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 320;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 1 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 384;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 1 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 448;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 2 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 512;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 2 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 576;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 2 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 640;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 2 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 704;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 3 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 768;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 3 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 832;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 3 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 896;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 3 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 960;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 4 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1024;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 4 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1088;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 4 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1152;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 4 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1216;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 5 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1280;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 5 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1344;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 5 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1408;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 5 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1472;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 6 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1536;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 6 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1600;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 6 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1664;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 6 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1728;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 7 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1792;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 7 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1856;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 7 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1920;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 7 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 1984;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 8 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2048;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 8 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2112;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 8 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2176;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 8 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2240;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 9 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2304;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 9 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2368;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 9 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2432;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 9 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2496;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 10 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2560;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 10 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2624;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 10 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2688;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 10 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2752;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 11 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2816;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 11 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2880;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 11 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 2944;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 11 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3008;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 12 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3072;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 12 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3136;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 12 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3200;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 12 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3264;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 13 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3328;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 13 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3392;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 13 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3456;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 13 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3520;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 14 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3584;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 14 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3648;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 14 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3712;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 14 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3776;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 15 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3840;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 15 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3904;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 15 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 3968;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 15 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4032;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 16 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4096;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 16 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4160;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 16 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4224;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 16 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4288;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 17 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4352;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 17 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4416;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 17 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4480;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 17 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4544;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 18 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4608;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 18 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4672;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 18 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4736;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 18 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4800;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 19 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4864;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 19 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4928;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 19 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 4992;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 19 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5056;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 20 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5120;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 20 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5184;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 20 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5248;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 20 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5312;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 21 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5376;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 21 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5440;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 21 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5504;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 21 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5568;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 22 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5632;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 22 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5696;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 22 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5760;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 22 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5824;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 23 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5888;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 23 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 5952;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 23 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6016;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 23 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6080;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 24 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6144;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 24 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6208;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 24 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6272;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 24 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6336;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 25 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6400;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 25 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6464;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 25 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6528;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 25 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6592;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 26 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6656;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 26 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6720;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 26 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6784;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 26 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6848;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 27 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6912;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 27 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 6976;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 27 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7040;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 27 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7104;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 28 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7168;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 28 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7232;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 28 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7296;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 28 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7360;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 29 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7424;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 29 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7488;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 29 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7552;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 29 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7616;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 30 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7680;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 30 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7744;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 30 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7808;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 30 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7872;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 31 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 7936;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 31 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8000;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 31 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8064;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 31 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8128;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 32 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8192;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 32 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8256;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 32 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8320;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 32 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8384;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 33 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8448;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 33 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8512;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 33 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8576;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 33 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8640;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 34 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8704;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 34 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8768;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 34 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8832;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 34 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8896;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 35 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 8960;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 35 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9024;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 35 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9088;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 35 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9152;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 36 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9216;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 36 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9280;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 36 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9344;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 36 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9408;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 37 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9472;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 37 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9536;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 37 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9600;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 37 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9664;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 38 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9728;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 38 && rxSecondArray[0] == 9)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9792;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 38 && rxSecondArray[0] == 10)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9856;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 38 && rxSecondArray[0] == 11)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9920;
    return;
  }
  if (rxFirstArray[0] == 6 && rxFirstArray[1] == 39 && rxSecondArray[0] == 8)
  {
    addressInt = rxSecondArray[1];
    addressInt += 9984;
    return;
  }
}

void SearchAddressCode()
{
  if (rxFirstArray[0] == 6)
  {
    DetectAddressCode();
  }
  if (rxFirstArray[0] == 4)
  {
    if (rxSecondArray[0] == 8 | rxSecondArray[0] == 9)
    {
      DetectAddressCode();
    }
  }
  if (addressInt > 0)
  {
    Serial.print("Loco=");
    Serial.print(addressInt);
    Serial.println("");
  }
}

//===================================== ISR GPIO15 ================

void IRAM_ATTR GPIO15ToLow()
{
  detachInterrupt(digitalPinToInterrupt(railComInt));
  IniCutoutTimer();                                     // Start cutout timer one time
  intStart = true;
}

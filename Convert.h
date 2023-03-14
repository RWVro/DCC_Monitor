

bool test_4_8Code = true;
bool test_4_8Decimal = true;

int inByte;

//============================== 4-8 Table conversion ======================

int decodeArray[68] = {172, 170, 169, 165, 163, 166, 156, 154, 153, 149, 147, 150, 142, 141, 139, 177, 178, 180, 184, 116,
                       114, 108, 106, 105, 101, 99, 102, 92, 90, 89, 85, 83, 86, 78, 77, 75, 71, 113, 232, 228, 226, 209, 201,
                       197, 216, 212, 210, 202, 198, 204, 120, 23, 27, 29, 30, 46, 54, 58, 39, 43, 45, 53, 57, 51, 15, 240, 225, 31
                      };       // 31 is end of table (0001 1111)


int convertArray[67] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
                        27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
                        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66
                       };

//====================================== Convert 4_8 Table to dec. ====================

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

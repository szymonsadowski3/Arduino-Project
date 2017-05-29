#define BLANK -1
#define LIGHT 2
#define MED 1
#define DARK 0

byte nums_syms[] = {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110, B11101110, B00111110, B10011100, B01111010, B10011110, B10001110};
int TEXT[6] = {1,2,3,4,5,6};
int DISP = 1000;
byte BRIGHT[] = {0, 1, 1, 2};

byte pins[4] = {10,6,5,3};
//byte segments[7] = {2,4,7,8,11,12,13};
byte segments[7] = {13,12,11,8,7,4,2};


byte getBnessDuration(byte input) {
  byte bness_map[] = {3, 2, 1};

  return bness_map[input];
}



bool getNthBitFromByte(byte n, byte a) {
  return ((a >> n)  & 0x01);
}

void disp(byte number, byte dig) {
  byte digit = pins[dig];
  
  pinMode(digit, OUTPUT);

  for (byte i = 0; i < 7; ++i) {
    //index = 13 - i
    if(getNthBitFromByte(7 - i, number)) {
      pinMode(segments[i], OUTPUT); 
      digitalWrite(segments[i], LOW); 
    }
  }

  analogWrite(digit, 255);
}

void dispNum(char num, byte dig) {
  byte to_disp = B00000000;
  
  if (num != -1)
    to_disp = nums_syms[num];
  
  disp(to_disp, dig);
}

void disp4Nums(char fst, char snd, char trd, char fou, int duration, byte DEL_INTER = 1) {
  int counter = 0;

  while(counter < duration) {
    dispNum(fst, 0);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(0);
    
    dispNum(snd, 1);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(1);
    
    dispNum(trd, 2);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(2);
    
    dispNum(fou, 3);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(3);
  }
}

void disp4NumsBrightness(byte fst, byte snd, byte trd, byte fou, byte bness_fst, byte bness_snd, byte bness_trd, byte bness_fou, int duration, byte DEL_INTER = 1) {
  int counter = 0;
  int iteration = 0;

  byte period_fst = getBnessDuration(bness_fst);
  byte period_snd = getBnessDuration(bness_snd);
  byte dur_trd = getBnessDuration(bness_trd);
  byte dur_fou = getBnessDuration(bness_fou);

  while(counter < duration) {
    dispNum(fst, 0);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(0);
    
    dispNum(snd, 1);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(1);
    
    dispNum(trd, 2);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(2);
    
    dispNum(fou, 3);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(3);
    ++iteration;
  }
}

//void intro(byte fst, byte snd, byte trd, byte fou) {
//  disp4Nums(BLANK, BLANK, BLANK, fst, DISP);
//  disp4Nums(BLANK, BLANK, fst, snd, DISP);
//  disp4Nums(BLANK, fst, snd, trd, DISP);
//  disp4Nums(fst, snd, trd, fou, DISP);
//}

void dispCycle() {
  //INTRO
  disp4Nums(BLANK, BLANK, BLANK, TEXT[0], DISP);
  disp4Nums(BLANK, BLANK, TEXT[0], TEXT[1], DISP);
  disp4Nums(BLANK, TEXT[0], TEXT[1], TEXT[2], DISP);
  disp4Nums(TEXT[0], TEXT[1], TEXT[2], TEXT[3], DISP);

  //MED
  disp4Nums(TEXT[1], TEXT[2], TEXT[3], TEXT[4], DISP);
  disp4Nums(TEXT[2], TEXT[3], TEXT[4], TEXT[5], DISP);

  //OUTRO
  disp4Nums(TEXT[3], TEXT[4], TEXT[5], BLANK, DISP);
  disp4Nums(TEXT[4], TEXT[5], BLANK, BLANK, DISP);
  disp4Nums(TEXT[5], BLANK, BLANK, BLANK, DISP);
  disp4Nums(BLANK, BLANK, BLANK, BLANK, DISP);
}

void dispCycleWithBness() {
  //INTRO
  disp4NumsBrightness(BLANK, BLANK, BLANK, TEXT[0], BRIGHT[0], BRIGHT[1], BRIGHT[2], BRIGHT[3], DISP);
  disp4NumsBrightness(BLANK, BLANK, TEXT[0], TEXT[1], BRIGHT[0], BRIGHT[1], BRIGHT[2], BRIGHT[3], DISP);
  disp4NumsBrightness(BLANK, TEXT[0], TEXT[1], TEXT[2], BRIGHT[0], BRIGHT[1], BRIGHT[2], BRIGHT[3], DISP);
  disp4NumsBrightness(TEXT[0], TEXT[1], TEXT[2], TEXT[3], BRIGHT[0], BRIGHT[1], BRIGHT[2], BRIGHT[3], DISP);

  //MED
  disp4NumsBrightness(TEXT[1], TEXT[2], TEXT[3], TEXT[4], BRIGHT[0], BRIGHT[1], BRIGHT[2], BRIGHT[3], DISP);
  disp4NumsBrightness(TEXT[2], TEXT[3], TEXT[4], TEXT[5], BRIGHT[0], BRIGHT[1], BRIGHT[2], BRIGHT[3], DISP);

  //OUTRO
  disp4NumsBrightness(TEXT[3], TEXT[4], TEXT[5], BLANK, BRIGHT[0], BRIGHT[1], BRIGHT[2], BRIGHT[3], DISP);
  disp4NumsBrightness(TEXT[4], TEXT[5], BLANK, BLANK, BRIGHT[0], BRIGHT[1], BRIGHT[2], BRIGHT[3], DISP);
  disp4NumsBrightness(TEXT[5], BLANK, BLANK, BLANK, BRIGHT[0], BRIGHT[1], BRIGHT[2], BRIGHT[3], DISP);
  disp4NumsBrightness(BLANK, BLANK, BLANK, BLANK, BRIGHT[0], BRIGHT[1], BRIGHT[2], BRIGHT[3], DISP);
}

void turnOff(byte dig)
{
  byte digit = pins[dig];

  for (byte i = 0; i < 7; ++i) {
      pinMode(segments[i], OUTPUT); 
      digitalWrite(segments[i], HIGH); 
  }
  
  analogWrite(digit, 0);
}

void setup()
{
  Serial.begin(9600);

//  for(int i = 0; i < 3; ++i) {
//    disp4Nums(TEXT[i], TEXT[i+1], TEXT[i+2], TEXT[i+3]);
//    delay(DISP);
//  }

//dispNum(1, 0);
//delay(1000);
//turnOff(0);
//delay(1000);
//dispNum(2, 3);
//delay(1000);
//turnOff(1);
//dispNum(1, 0);
//delay(1000);
//turnOff(1);
//intro(1, 3, 5, 7);
dispCycle();
//disp4NumsBrightness(1,3,5,7,0,1,1,2, 10000);
//disp4Nums(BLANK,3,BLANK,7, 1000);
}

void loop()
{
}
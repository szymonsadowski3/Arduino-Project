#define BLANK -1

#define LIGHT 2
#define MED 1
#define DARK 0

//HARDWARE TABLES
byte pins[4] = {10,6,5,3};
byte segments[7] = {13,12,11,8,7,4,2};

//HELPER TABLES
byte nums_syms[] = {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110, B11101110, B00111110, B10011100, B01111010, B10011110, B10001110, B00000000};
byte BNESS_MAPPING[] = {10, 60, 255};

//USER COMMANDS
char TEXT[] = "0f34ec";
char BRIGHT[] = "0000";
int DISP = 1000;


//IMPLEMENTATION
byte getBrightForSegment(byte n) {
  char extracted_char = BRIGHT[n];
  
  return extracted_char - 48;
}

char getTextCharacter(byte n) {
  if(n==-1)
    return 16;
  
  int decimal = 0;
  char extracted_char = tolower(TEXT[n]);
  
  if(extracted_char > 57) {
    decimal = extracted_char - 87;
  } else {
    decimal = extracted_char - 48;
  }

  return decimal;
}

bool getNthBitFromByte(byte n, byte a) {
  return ((a >> n)  & 0x01);
}

void disp(byte number, byte dig, byte bness=255) {
  byte digit = pins[dig];
  
  pinMode(digit, OUTPUT);

  for (byte i = 0; i < 7; ++i) {
    //index = 13 - i
    if(getNthBitFromByte(7 - i, number)) {
      pinMode(segments[i], OUTPUT); 
      digitalWrite(segments[i], LOW); 
    }
  }

//  Serial.print(bness);
//  Serial.print("\n");
  
  analogWrite(digit, bness);
}

void dispNum(char num, byte dig, byte bness=255) {
  byte to_disp = B00000000;
  
  if (num != -1)
    to_disp = nums_syms[num];
  
  disp(to_disp, dig, bness);
}

void disp4NumsBrightness(byte fst, byte snd, byte trd, byte fou, byte bness_fst, byte bness_snd, byte bness_trd, byte bness_fou, int duration, byte DEL_INTER = 1) {
  if(inputCheck()) {
    fst = getTextCharacter(0);
    snd = getTextCharacter(1);
    trd = getTextCharacter(2);
    fou = getTextCharacter(3);

    bness_fst = getBrightForSegment(0);
    bness_snd = getBrightForSegment(1);
    bness_trd = getBrightForSegment(2);
    bness_fou = getBrightForSegment(3);

    duration = DISP;
  }
  
  int counter = 0;
  int iteration = 0;
  
  while(counter < duration) {
    dispNum(fst, 0, BNESS_MAPPING[bness_fst]);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(0);
    
    dispNum(snd, 1, BNESS_MAPPING[bness_snd]);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(1);
    
    dispNum(trd, 2, BNESS_MAPPING[bness_trd]);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(2);
    
    dispNum(fou, 3, BNESS_MAPPING[bness_fou]);

    counter += DEL_INTER;
    delay(DEL_INTER);
    turnOff(3);
    ++iteration;
  }
}

void dispCycleWithBness() {
  //INTRO
  disp4NumsBrightness(BLANK, BLANK, BLANK, getTextCharacter(0), getBrightForSegment(0), getBrightForSegment(1), getBrightForSegment(2), getBrightForSegment(3), DISP);
  disp4NumsBrightness(BLANK, BLANK, getTextCharacter(0), getTextCharacter(1), getBrightForSegment(0), getBrightForSegment(1), getBrightForSegment(2), getBrightForSegment(3), DISP);
  disp4NumsBrightness(BLANK, getTextCharacter(0), getTextCharacter(1), getTextCharacter(2), getBrightForSegment(0), getBrightForSegment(1), getBrightForSegment(2), getBrightForSegment(3), DISP);
  disp4NumsBrightness(getTextCharacter(0), getTextCharacter(1), getTextCharacter(2), getTextCharacter(3), getBrightForSegment(0), getBrightForSegment(1), getBrightForSegment(2), getBrightForSegment(3), DISP);

  //MED
  disp4NumsBrightness(getTextCharacter(1), getTextCharacter(2), getTextCharacter(3), getTextCharacter(4), getBrightForSegment(0), getBrightForSegment(1), getBrightForSegment(2), getBrightForSegment(3), DISP);
  disp4NumsBrightness(getTextCharacter(2), getTextCharacter(3), getTextCharacter(4), getTextCharacter(5), getBrightForSegment(0), getBrightForSegment(1), getBrightForSegment(2), getBrightForSegment(3), DISP);

  //OUTRO
  disp4NumsBrightness(getTextCharacter(3), getTextCharacter(4), getTextCharacter(5), BLANK, getBrightForSegment(0), getBrightForSegment(1), getBrightForSegment(2), getBrightForSegment(3), DISP);
  disp4NumsBrightness(getTextCharacter(4), getTextCharacter(5), BLANK, BLANK, getBrightForSegment(0), getBrightForSegment(1), getBrightForSegment(2), getBrightForSegment(3), DISP);
  disp4NumsBrightness(getTextCharacter(5), BLANK, BLANK, BLANK, getBrightForSegment(0), getBrightForSegment(1), getBrightForSegment(2), getBrightForSegment(3), DISP);
  disp4NumsBrightness(BLANK, BLANK, BLANK, BLANK, getBrightForSegment(0), getBrightForSegment(1), getBrightForSegment(2), getBrightForSegment(3), DISP);
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
  //dispCycleWithBness();
}

void loop()
{
  dispCycleWithBness();
}

bool inputCheck() {
  String a;
  while(Serial.available()) {
    a = Serial.readString();// read the incoming data as string
    parseCommand(a);
  }

  return a.length()>0;
}

//PARSER
void parseCommand(String input) {
  char indicator = input[0];
  int inp_len = input.length();

  if(indicator=='T') {
    if(inp_len!=8) { //8 ponieważ \n na końcu
      Serial.println("T command error (LEN Should be exactly 7!)");
      Serial.println(inp_len);
    }

    for(int i = 1; i < 7; ++i) {
      if(!isalnum(input[i])) {
        Serial.println("T command error (NON-ALNUM-CHAR : assuming 0!)");
        TEXT[i-1] = '0';
      } else {
        TEXT[i-1] = input[i];
      }
    }
  } else if(indicator=='D') {
    if(inp_len!=6) { //6 ponieważ \n na końcu
      Serial.println("D_cmd_err");
    } else {
      if(input.substring(1,6).toInt()==0) {
        Serial.println("D_cmd_err");
      } else {
        DISP = input.substring(1,6).toInt();
      }
    }
  } else if(indicator=='B') {
    if(inp_len!=6) {
      Serial.println("D command error (LEN Should be exactly 5!)");
    } else {
      for(int i = 1; i < 5; ++i) {
        if(input[i] >= '0' && input[i] <= '2'){
          BRIGHT[i-1] = input[i];
        } else {
          Serial.println("B_cmd_err (num must be [0|1|2]!)");
        }
      }
    }
  } else {
    Serial.println("Parsing command error at 1st character (unrecognized prefix)");
  }
}


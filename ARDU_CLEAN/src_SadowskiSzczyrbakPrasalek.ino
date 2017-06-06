#define BLANK -1

//HARDWARE
byte pins[4] = {10,6,5,3}; //KOLEJNOŚĆ POŁĄCZENIA DIGITS
byte segments[7] = {13,12,11,8,7,4,2}; //KOLEJNOŚĆ POŁĄCZENIA SEGMENTÓW

//POMOCNICZE TABELE
//nums_syns -> binarna representacja heksadecymalnych wartości
byte nums_syms[] = {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110, B11101110, B00111110, B10011100, B01111010, B10011110, B10001110, B00000000};
byte BNESS_MAPPING[] = {10, 60, 255}; //mapowanie jasności dla analogWrite, gdzie 0-dark 1-medium 2-light

//KOMENDY UŻYTKOWNIKA
char TEXT[] = "0f34ec";
char BRIGHT[] = "0000";
short int DISP = 1000;

char TEXT_BUFF[14] = {-1, -1, -1, -1, -1, -1,-1, -1, -1, -1, -1, -1,-1,-1};
byte TEXT_BUFF_INDICATOR = 0;

//IMPLEMENTACJA
//Ta funkcja wyciąga symbol jasności [0|1|2] z tabeli 'BRIGHT'
#define getBrightForSegment(n) BRIGHT[n]-48

/*
 * Updatuje TEXT_BUFF w odniesieniu do zmiennej TEXT
 */
void updateTextBuff() {
  for(byte i = 4; i < 10; ++i) {
    TEXT_BUFF[i] = getTextCharacter(i-4);
  }
}

/*
 * Przesuwa wskaźnik aktualnej pozycji w TEXT_BUFF o 1 lub zeruje go
 */
void moveTextBuff() {
  if(TEXT_BUFF_INDICATOR==9) {
    TEXT_BUFF_INDICATOR=0;
    return;
  }
  
  ++TEXT_BUFF_INDICATOR;
}

/*
 * Zwraca n-tą wartość całkowitą pobraną z 'TEXT'
 */
char getTextCharacter(byte n) {
  if(n==-1)
    return -1;
  
  char extracted_char = tolower(TEXT[n]);
  
  if(extracted_char > 57) {
    return extracted_char - 87;
  } else {
    return extracted_char - 48;
  }
}

/*
 * Wyciąga n-ty bit z bajta
 */
bool getNthBitFromByte(byte n, byte a) {
  return ((a >> n)  & 0x01);
}

/*
 * Wyświetla te segmenty, gdzie ustawiony jest bit w 'number'. Wyświetla w segmencie o nr. 'dig' o zadanej jasności 'bness'
 */
void disp(byte number, byte dig, byte bness=255) {
  for (byte i = 0; i < 7; ++i) {
    if(getNthBitFromByte(7 - i, number)) {
      digitalWrite(segments[i], LOW); 
    }
  }
  
  analogWrite(pins[dig], bness);
}

/*
 * Wyświetla wartość całkowitą podaną w 'num' np. jak podamy num=12 to wyswietli 'c'
 */
void dispNum(char num, byte dig, byte bness=255) {
  if (num != -1)
    disp(nums_syms[num], dig, bness);
  
  disp(B00000000, dig, bness);
}

/*
 * Wyswietla naraz 4 liczby całkowite podane w zmiennej TEXT_BUFF
 * o jasnościach podanych odpowiednio w tablicy BRIGHT
 * duration - czas trwania całego wyświetlania
 * DEL_INTER = delay interval czyli częstotliwość odświeżania
 * Gdy podamy na danym miejscu -1 to nic się tam nie wyświetli
 */
void disp4NumsBrightness(short int duration, byte DEL_INTER = 6) {
  short int counter = 0;

  while(counter < duration) {
    //if(counter%96==0)
    inputCheck();

    for(byte i = 0; i < 4; ++i) {
      dispNum(TEXT_BUFF[TEXT_BUFF_INDICATOR + i], i, BNESS_MAPPING[getBrightForSegment(i)]);

      counter += DEL_INTER;
      delay(DEL_INTER);
      turnOff(i);
    }
  }
}

/*
 * Wyświetla cykl pojawiania się 6 liczb hexadecymalnych od prawej do lewej z wyłanianiem się
 */
void dispCycleWithBness() {
  disp4NumsBrightness(DISP);
  moveTextBuff();
}

/*
 * "Czyści" cały digit o podanym numerze
 */
void turnOff(byte dig)
{
  for (byte i = 0; i < 7; ++i) {
      digitalWrite(segments[i], HIGH); 
  }
  
  analogWrite(pins[dig], 0);
}

void setup()
{
  Serial.begin(9600);
  for (byte i = 0; i < 7; ++i) {
      pinMode(segments[i], OUTPUT); 
  }

  for (byte i = 0; i < 4; ++i) {
      pinMode(pins[i], OUTPUT); 
  }

  updateTextBuff();
}

void loop()
{
  dispCycleWithBness();
}

/*
 * Funkcja która sprawdza, czy na wejściu serialowym jest jakiś input
 * jeśli jest to zwraca true, false w przeciwnym wypadku
 */
bool inputCheck() {
  String a;
  while(Serial.available()) {
    a = Serial.readString();
    if(a.length()>4)
      parseCommand(a);
  }

  return a.length()>4;
}

/*
 * Funkcja wcielająca w życie komendy podane przez użytkownika, parser
 */
void parseCommand(String input) {
  int inp_len = input.length();

  if(input[0]=='T') {
    if(inp_len!=8) { //8 ponieważ \n na końcu
      Serial.println("T| LEN!=7");
    }

    for(int i = 1; i < 7; ++i) {
      if(!isalnum(input[i])) {
        Serial.println("T| NON-ALNUM-CHAR, assuming 0!");
        TEXT[i-1] = '0';
      } else {
        TEXT[i-1] = input[i];
      }
    }

    updateTextBuff();
    
  } else if(input[0]=='D') {
    if(inp_len!=6) { //6 ponieważ \n na końcu
      Serial.println("D| LEN!=5");
    } else {
      if(input.substring(1,6).toInt()==0) {
        Serial.println("D| parse_int err!");
      } else {
        DISP = input.substring(1,6).toInt();
      }
    }
  } else if(input[0]=='B') {
    if(inp_len!=6) { //6 ponieważ \n na końcu
      Serial.println("D| LEN!=5");
    } else {
      for(int i = 1; i < 5; ++i) {
        if(input[i] >= '0' && input[i] <= '2'){
          BRIGHT[i-1] = input[i];
        } else {
          Serial.println("B| nums must be in [0|1|2]!");
        }
      }
    }
  } else {
    Serial.println("unrecognized prefix!");
  }
}

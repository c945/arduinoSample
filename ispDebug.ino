// AT90S2313 ISP Test Program.
// 2019.07.21

#define PIN_RESET 10
#define PIN_MOSI  11
#define PIN_MISO  12
#define PIN_SCK   13

#define DELAY 1
uint8_t CMD_PGM[] = { 0xac, 0x53, 0, 0 };
uint8_t CMD_VF0[] = { 0x30, 0x00, 0, 0 };
uint8_t CMD_VF1[] = { 0x30, 0x00, 1, 0 };
uint8_t CMD_VF2[] = { 0x30, 0x00, 2, 0 };

void setup() {
  Serial.begin(9600);
  digitalWrite(PIN_RESET, HIGH);
  digitalWrite(PIN_SCK, LOW);
  digitalWrite(PIN_MOSI, LOW);
  digitalWrite(PIN_MISO, HIGH);

  // RESET:HIGH
  // SCK  :LOW
  // MOSI :LOW
  pinMode(PIN_RESET,OUTPUT);
  pinMode(PIN_SCK, OUTPUT);
  pinMode(PIN_MISO, INPUT_PULLUP);
  pinMode(PIN_MOSI, OUTPUT);

  // RESET:LOW
  digitalWrite(PIN_RESET, LOW);
  delay(20);

  snd(CMD_PGM);

  delay(20);
  snd(CMD_VF0);
  
  delay(20);
  snd(CMD_VF1);
  
  delay(20);
  snd(CMD_VF2);
  
  pinMode(PIN_RESET, INPUT);
  pinMode(PIN_SCK, INPUT);
  pinMode(PIN_MOSI, INPUT);
  pinMode(PIN_MISO, INPUT);
}

void loop() {
}

void snd(uint8_t *ptr)
{
  int i,j;
  uint8_t c, n;
  uint8_t res[4];

  //Serial.println("-----snd");
  for(i=0; i<4; i++) {    // 4 bytes
    c = ptr[i];
    res[i] = 0;
    for(j=0; j<8; j++) {  // 8 bit
      //Serial.print(i,HEX);
      //Serial.print(j,DEC);
      //Serial.print(" ");
      if(c & 0x80) {  // HIGH
        digitalWrite(PIN_MOSI, HIGH);
        //Serial.print(HIGH);
      } else {
        digitalWrite(PIN_MOSI, LOW);
        //Serial.print(LOW);
      }
      //Serial.print(" ");
      c <<= 1;
      delayMicroseconds(DELAY);
      digitalWrite(PIN_SCK, HIGH);
      delayMicroseconds(DELAY);
      n = digitalRead(PIN_MISO);
      digitalWrite(PIN_SCK, LOW);
      delayMicroseconds(DELAY);
      //Serial.print( n );
      //Serial.println("");
      res[i] <<= 1;
      if(n) {
        res[i] |= 0x1;
      }
    }
        //Serial.println("");
  }

  serial4(ptr, "\t");
  serial4(res, "\n");
}

void  serial4(uint8_t *ptr, char *str)
{
  for(int i=0; i<4; i++) {
    if(ptr[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(ptr[i], HEX);
    Serial.print(" ");
  }
  Serial.print(str);
}

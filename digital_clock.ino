// control buttons
#define setButton 2
#define OKButton  3
// shift register control pins 
#define SER   5   // serial pin
#define RCLK  6   // latch pin
#define SRCLK 7   // clock pin

// digit display enable pins 
#define dig1 8 
#define dig2 9
#define dig3 10
#define dig4 11

// shift register byte for each number
#define zero  0x7E
#define one   0x30
#define two   0x6D
#define three 0x79
#define four  0x27
#define five  0x5B
#define six   0x5F
#define seven 0x70
#define eight 0x7F
#define nine  0x7B

void setup() {
  sei();
  for (uint8_t i = 2; i < 9; i++) {
    pinMode(i, OUTPUT);
  }
  for (uint8_t i = 2; i < 9; i++) {
    digitalWrite(i, LOW);
  }
}
void loop() {
  SRbyte((uint8_t)0b01010101);
  delay(1000);
  SRbyte((uint8_t)0b10101010);
  delay(1000);
}
void displayDigit(uint8_t digit, uint8_t pos) {
  switch (digit) {
    case 1 :
      SRbyte(one);
      break;
    case 2 :
      SRbyte(two);
      break;
    case 3 :
      SRbyte(three);
      break;
    case 4 :
      SRbyte(four);
      break;
    case 5 :
      SRbyte(five);
      break;
    case 6 :
      SRbyte(six);
      break;
    case 7 :
      SRbyte(seven);
      break;
    case 8 :
      SRbyte(eight);
      break;
    case 9 :
      SRbyte(nine);
      break;
    default:
      SRbyte(zero);
  }
  PORTB &= ~( 0xF << 0);
  digitalWrite(1, 7 + pos);
}
void SRbyte(uint8_t dig) { 
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SRCLK, HIGH);
    (dig & (1 << i)) ? digitalWrite(SER, HIGH) : digitalWrite(SER, LOW);
    digitalWrite(SRCLK, LOW);
  }
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}
ISR(INT0_vect){
  
}

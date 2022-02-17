uint8_t setTime = 0x00;
// buttons variables
boolean buttonIsPressed;
// define variables for time counting using timer 1
uint8_t hour = 0, minute = 0, second = 0;
// control buttons
#define setButton 2 // 
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
  sei(); // activate the global interrupts
  pinMode(setButton, INPUT);
  pinMode(OKButton , INPUT);
  for (uint8_t i = 5; i < 12; i++) {
    pinMode(i, OUTPUT);
  }
  for (uint8_t i = 5; i < 12; i++) {
    digitalWrite(i, LOW);
  }
  timeCount();
}

void loop() {
  if (buttonpPressed(setButton) && setTime == 0x00)
    setTime = setTime << 1;
  if (setTime == 0x02) {
    cli();
    displayTime(hour, minute);
    if (buttonpPressed(setButton))
      setTime = setTime << 1;
    if (buttonpPressed(OKButton))
      hour++;
  }
  if (setTime == 0x04) {
    cli();
    displayTime(hour, minute);
    if (buttonpPressed(setButton))
      setTime = 0x00;
    if (buttonpPressed(OKButton))
      minute++;
  }
  else
    displayTime(hour, minute);
}

// function to read buttons state
boolean buttonpPressed(uint8_t pin) {
  if (digitalRead(pin)) {
    delay(10);
    if (digitalRead(pin)) {
      while (digitalRead(pin));
      buttonIsPressed = true ;
    } else {
      buttonIsPressed = false;
    }
  }
  return buttonIsPressed ;
}
// function to configer the the timer/counter1
void timeCount() {
 cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();
}
// function to display a time in a 7segment 4 digit display LED or our clock in this cas
void displayTime(uint8_t hh, uint8_t mm) {
  if (hh < 10) {
    displayDigit(0, 4);
    displayDigit(hh, 3);
  } else {
    displayDigit(abs(hh / 10), 4);
    displayDigit(hh % 10, 3);
  }
  if (mm < 10) {
    displayDigit(0, 2);
    displayDigit(hh, 1);
  } else {
    displayDigit(abs(mm / 10), 2);
    displayDigit(mm % 10, 1);
  }
}
// function to display a digit in a certain position
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
// function to send a byte through a register
void SRbyte(uint8_t dig) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SRCLK, HIGH);
    (dig & (1 << i)) ? digitalWrite(SER, HIGH) : digitalWrite(SER, LOW);
    digitalWrite(SRCLK, LOW);
  }
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}
// time counter using timer1
ISR(TIMER1_COMPA_vect) {
  if (second < 60)
    second++;
  else {
    second = 0;
    if (minute < 60)
      minute++;
    else {
      minute = 0;
      hour < 24 ? hour++ : hour = 0;
    }
  }
}

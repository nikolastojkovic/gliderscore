//This version installed on large board
//Arduino Pro Mini
//uses HC

#include <Adafruit_NeoPixel.h>
#define LED_PIN1    5
#define LED_COUNT1 58
Adafruit_NeoPixel strip1(LED_COUNT1, LED_PIN1, NEO_GRB + NEO_KHZ800);

#include <SoftwareSerial.h>
SoftwareSerial mySerial(A1, A2); // RX, TX

byte groupround = 0;
byte state;
byte number;
byte onedigit;
byte rondone;
byte rondten;
byte groupone;
byte groupten;
byte thousands;   //timer, tens of minutes
byte hundreds;    //timer, ones of minutes
byte tens;        //timer, tens of seconds
byte ones;        //timer, ones of seconds
byte sequence;
byte offset;
uint32_t colour;

void setup() {
  // put your setup code here, to run once:
  strip1.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  //Serial.begin(9600);
  mySerial.begin(9600);

}

void loop() {
  if (mySerial.available())
  {
    GetGSStuff(rondten, rondone, groupten, groupone, thousands, hundreds, tens, ones, state );
    strip1.clear();

    if (sequence == 3) {            // three loops (seconds) then display other group/round
      groupround = !groupround;
      sequence = 0;
    }
    ++sequence;

    if (state == 87 ) { //if its working time LEDs are green
//      colour = strip1.Color(0, 255, 125); //R(0-255),G(0-255),B(0-255)
      colour = strip1.Color(0, 0, 255); //R(0-255),G(0-255),B(0-255) // LEDs green color
//      colour = strip1.Color(255, 255, 255); //R(0-255),G(0-255),B(0-255) // LEDs white color
    }
    else { //if its prep time LEDs are red
//      colour = strip1.Color( 255, 34 , 0); //R(0-255),G(0-255),B(0-255)
      colour = strip1.Color( 255, 0 , 0); //R(0-255),G(0-255),B(0-255)
    }

//    if (groupround == 1) {
//      number = rondone;
//      onedigit = rondten;
//      offset = 46;
//      numbershow();
//      strip1.fill(colour, offset + 20, 2); // displays r
//      strip1.fill(colour, offset + 28, 2);
//    }
//    else {
//      number = groupone;
//      onedigit = groupten;
//      offset = 46;
//      numbershow();
//      strip1.fill(colour, offset + 16, 10); //displays g
//    }

    number = ones;
    onedigit = 0;
    offset = 0;
    numbershow();
//
//    if (tens != 0 || hundreds > 0 || thousands > 0) { // does not blank if there are minutes
//      number = tens;
//      onedigit = 0;
//      offset = 14;
//      numbershow();
//    }

      number = tens;
      onedigit = 0;
      offset = 14;
      numbershow();
//
//    if (hundreds != 0 || thousands > 0) { // does not blank if there are tens of minutes
//      number = hundreds;
//      onedigit = thousands;
//      offset = 28;
//      numbershow();
//    }

      number = hundreds;
      onedigit = thousands;
      offset = 28;
      numbershow();

      number = thousands;
      onedigit = 1;
      offset = 42;
      numbershow();

      strip1.setBrightness(255); // brightness of the LEDs o-255, min-max
      strip1.fill(colour, offset + 14, 4);

    strip1.show();

  }

}

void numbershow() {
  switch (number) {
    case 0:
      strip1.fill(colour, offset + 0, 12);
      break;
    case 1:
      strip1.fill(colour, offset + 0, 2);
      strip1.fill(colour, offset + 10, 2);
      break;
    case 2:
      strip1.fill(colour, offset + 2, 4);
      strip1.fill(colour, offset + 8, 6);
      break;
    case 3:
      strip1.fill(colour, offset + 0, 4);
      strip1.fill(colour, offset + 8, 6);
      break;
    case 4:
      strip1.fill(colour, offset + 0, 2);
      strip1.fill(colour, offset + 6, 2);
      strip1.fill(colour, offset + 10, 4);
      break;
    case 5:
      strip1.fill(colour, offset + 0, 4);
      strip1.fill(colour, offset + 6, 4);
      strip1.fill(colour, offset + 12, 2);
      break;
    case 6:
      strip1.fill(colour, offset + 0, 10);
      strip1.fill(colour, offset + 12, 2);
      break;
    case 7:
      strip1.fill(colour, offset + 0, 2);
      strip1.fill(colour, offset + 8, 4);
      break;
    case 8:
      strip1.fill(colour, offset + 0, 14);
      break;
    case 9:
      strip1.fill(colour, offset + 0, 4);
      strip1.fill(colour, offset + 6, 8);
      break;
  }

//  if (onedigit == 1) {
//    strip1.fill(colour, offset + 14, 4);
//  }
}

void        GetGSStuff(byte& rondten, byte& rondone, byte& groupten, byte& groupone,
                       byte& thousands, byte& hundreds, byte& tens, byte& ones, byte& state) { //parses the string from gliderscore
  boolean GotString = false;
  char InChar;
  char InString[20];
  byte j = 0;
  while (!GotString) {
    if (mySerial.available()) {
      InChar = mySerial.read();
      InString[j] = InChar;
      j += 1;
      if (InChar == '\r') {
        GotString = true;
      }
    }
  }

  //Serial.println(InString); this line tests the full received string
  // the -48 changes the recived char from ascii to a number for later use in driving LED strings.
  rondten = (byte)InString[1] - 48;
  rondone = (byte)InString[2] - 48;
  groupten = (byte)InString[4] - 48;
  groupone = (byte)InString[5] - 48;
  thousands = (byte)InString[7] - 48;
  hundreds = (byte)InString[8] - 48;
  tens = (byte)InString[9] - 48;
  ones = (byte)InString[10] - 48;
  state = (byte)InString[11];

}

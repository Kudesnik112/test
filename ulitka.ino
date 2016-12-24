#include <IRremote.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define boff 551505585
#define bloop 4294967295
#define bplus 551519865
#define bminus 551536185
#define bcolor 551521905
#define bprog 551525475

//#define bplus 16599733
//#define boff 16630333
//#define bcolor 16591573
//#define bminus 16632373

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      6

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int RECV_PIN = 3;

IRrecv irrecv(RECV_PIN);

decode_results results;

long blast;
int decColour = 0;
int vmaxa;
int cnt = 0;
int vmax = 255;
int vmina;
int vmin;
float cnst;
float value = 1;
int i = 0;
boolean ledOn = 1;
boolean colorOn = 0;
int rgbColour[3];

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  rgbColour[0] = 255;
  rgbColour[1] = 255;
  rgbColour[2] = 255;
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
  setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);

}

void loop() {


  if (irrecv.decode(&results)) {
    //dump(&results);
    Serial.println(results.value);
    switch (results.value) {

      case boff:
        if (ledOn == 0) {
          setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
        } else {
          setColourRgb(0, 0, 0);
        }
        blast = boff;
        ledOn = !ledOn;
        goto brk;
reset:
        if (cnt > 10) {
          colorOn = 0;
          decColour = 0;
          ledOn = !ledOn;
          i = 0;
          rgbColour[0] = 255;
          rgbColour[1] = 255;
          rgbColour[2] = 255;
          cnt = 0;
          setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
        } else ++cnt;
brk:
        break;


      case bplus:
        blast = bplus;
bpls:
        value += value >= 1 ? 0 : 0.1;
        setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
        break;
      case bminus:
        blast = bminus;
bmns:
        value -= value <= 0 ? 0 : 0.1;

        setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
        break;
      case bcolor:
        blast = bcolor;
bclr:
        if (colorOn == 0) {
          rgbColour[0] = 255;
          rgbColour[1] = 0;
          rgbColour[2] = 0;
          colorOn = 1;
        }
        //        decColour = 0;
        //        i = 0;
        vcolorx(rgbColour);
        setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
        break;
      case bloop:
        irrecv.resume();
        if (blast == bminus) goto bmns;
        if (blast == bplus) goto bpls;
        if (blast == bcolor) goto bclr;
        if (blast == boff) goto reset;
        break;
        //      default:
    }
    irrecv.resume(); // Receive the next value
  }

}

void vcolorx(int rgbColour[]) {

  if (decColour < 3) {

    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    if ( i < 255 - 10 ) {
      Serial.print(i);
      Serial.print(":");
      Serial.print(255);
      Serial.print("\t");
      rgbColour[decColour] -= 10;
      rgbColour[incColour] += 10;
      i += 10;
    }
    else {
      i = 0;
      decColour += 1;
    }
  }
  else {
    decColour = 0;
  }
  return;
}


void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  
  red *= value;
  green *= value;
  blue *= value;
  
  for (int i = 0; i < NUMPIXELS; i++) {
  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  //  Serial.print(red);
  //  Serial.print(",");
  //  Serial.print(green);
  //  Serial.print(",");

  pixels.setPixelColor(i, pixels.Color(red, green, blue)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  }
}


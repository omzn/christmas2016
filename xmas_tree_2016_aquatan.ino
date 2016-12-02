#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 3
#define NUM_LEDS 32
#define BRIGHTNESS 45

#define NUM_RANDOMLED 7

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

uint8_t p0[] = {3, 2, 1};
uint8_t p1[] = {4, 5, 6, 7};
uint8_t p2[] = {12, 11, 10};
uint8_t p3[] = {13, 14, 15, 16};
uint8_t p4[] = {21, 20, 19};
uint8_t p5[] = {22, 23, 24, 25};
uint8_t p6[] = {30, 29, 28};
uint8_t p7[] = {31}; // yellow star
uint8_t p8[] = {8, 17, 26}; // white bottom
uint8_t p9[] = {0, 9, 18, 27}; // white bottom

int count = 0;
int fade = 0;
uint32_t r[NUM_RANDOMLED][5]; // number of random pixels, parameters

void setup() {
  setRandomSeed();
  Serial.begin(9600);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
  colorSet(strip.Color(0, 255, 0), p0, 3);
  colorSet(strip.Color(0, 255, 0), p1, 4);
  colorSet(strip.Color(0, 255, 0), p2, 3);
  colorSet(strip.Color(0, 255, 0), p3, 4);
  colorSet(strip.Color(0, 255, 0), p4, 3);
  colorSet(strip.Color(0, 255, 0), p5, 4);
  colorSet(strip.Color(0, 255, 0), p6, 3);
  colorSet(strip.Color(0, 255, 0), p7, 1);
  colorSet(strip.Color(255, 255, 0), p8, 3);
  colorSet(strip.Color(255, 255, 255), p9, 4);
  strip.show();
  delay(50);
  for (int i = 0; i < NUM_RANDOMLED; i++) {
    r[i][0] = 255;
  }
}


void loop() {
  // Some example procedures showing how to display to the pixels:
  if (count / 100 == 0) {
    fade = (count % 100) + 1;
  } else if (count / 100 == 1) {
    fade = 100 - ((count % 100) + 1);
  }

  fadePixels(p7, 1, strip.Color(0, 255, 0), strip.Color(255, 255, 255), fade);
  fadePixels(p8, 3, strip.Color(255, 255, 0), strip.Color(0, 0, 0), fade);
  fadePixels(p9, 4, strip.Color(0, 0, 0), strip.Color(255, 255, 255), fade);

  if (count % 40 == 0) {
    selectRandomLed(3, p3, 4);
    selectRandomLed(5, p5, 4);
    selectRandomLed(1, p1, 4);
  }
  if (count % 60 == 0) {
    selectRandomLed(2, p2, 3);
    selectRandomLed(4, p4, 3);
    selectRandomLed(0, p0, 3);
    selectRandomLed(6, p6, 3);
  }

  for (int i = 0; i < NUM_RANDOMLED; i++) {
    if (r[i][0] != 255) {
      //      r[i][1] += 2;
      r[i][2] ++;
      if (r[i][2] >= 100) {
        strip.setPixelColor(r[i][0], strip.Color(0, 255, 0));
        r[i][0] = 255;
        r[i][1] = 0;
        r[i][2] = 0;
        //       Serial.print("stopped id=");
        //       Serial.println(r[i][0]);
      } else {
        fadeAPixel(r[i][0], r[i][3], r[i][4], r[i][1] * r[i][2]);
      }
    }
  }

  strip.show();
  delay(25);
  count++;
  count %= 200; // 5s
}

int selectRandomLed(int i, uint8_t *pixels, int len) {
  //  Serial.print("random led fade =");
  //  Serial.println(r[i][2]);
  if (r[i][0] == 255) {
    r[i][0] = pixels[random(0, len)]; // led id
    r[i][1] = random(1, 4) * 2; // fade speed
    r[i][2] = 1; // local counter
    r[i][3] = strip.Color(random(0, 256), random(0, 256), random(0, 256)); // start color (green)
    r[i][4] = strip.Color(random(0, 256), random(0, 256), random(0, 256)); // end color
    //    Serial.print("set random led: id=");
    //    Serial.println(r[i][0]);
    //    Serial.print("slot = ");
    //    Serial.println(i);
    return 1;
  }
  return 0;
}

void fadeAPixel(uint8_t pix, uint32_t start_c, uint32_t end_c, int fadeVal) {
  float fadeMax = 100.0;
  int redVal, greenVal, blueVal;
  redVal = red(start_c)- (red(start_c) - red(end_c)) * float((cos((fadeVal/fadeMax)*PI)+1)/2);
  greenVal = green(start_c)- (green(start_c) - green(end_c)) * float((cos((fadeVal/fadeMax)*PI)+1)/2);
  blueVal = blue(start_c)- (blue(start_c) - blue(end_c)) * float((cos((fadeVal/fadeMax)*PI)+1)/2);
  strip.setPixelColor(pix, strip.Color(redVal, greenVal, blueVal));
}

void fadePixels(uint8_t *pixels, uint8_t len, uint32_t start_c, uint32_t end_c, int fadeVal) {
  float fadeMax = 100.0;
  int redVal, greenVal, blueVal;
  redVal = red(start_c)- (red(start_c) - red(end_c)) * float((cos((fadeVal/fadeMax)*PI)+1)/2);
  greenVal = green(start_c)- (green(start_c) - green(end_c)) * float((cos((fadeVal/fadeMax)*PI)+1)/2);
  blueVal = blue(start_c)- (blue(start_c) - blue(end_c)) * float((cos((fadeVal/fadeMax)*PI)+1)/2);
  for (int i = 0; i < len; i++) {
    strip.setPixelColor(pixels[i], strip.Color(redVal, greenVal, blueVal));
  }
}

void colorSet(uint32_t c, uint8_t *pixels, uint8_t len) {
  for (int i = 0; i < len; i++) {
    strip.setPixelColor(pixels[i], c);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void setRandomSeed()
{
  uint32_t seed;

  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  delay(1);

  for (int shifts = 3; shifts < 31; shifts += 3)
  {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }

  // Serial.println(seed);
  randomSeed(seed);
}

uint8_t red(uint32_t c) {
  return (c >> 8);
}
uint8_t green(uint32_t c) {
  return (c >> 16);
}
uint8_t blue(uint32_t c) {
  return (c);
}


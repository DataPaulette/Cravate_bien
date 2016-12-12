#include <elapsedMillis.h>
#include <Adafruit_NeoPixel.h>

elapsedMillis countMillis;

const int DATA_PIN = 4;   // P4 on the Digispark-ATTiny85
const int MIC_PIN = 1;    // P2 on the Digispark-ATTiny85
const int NUMPIXELS = 16; //

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

uint32_t myColors[] = {
  0x99FFCC, 0x99FF99, 0x99FF66, 0x99FF33, 0x99FF00, 0xCCFF33, 0xCCFF00, 0xFFFF00, 0xFFCC00, 0xCC9900, 0xFF9900, 0xFF6600, 0xFF3300, 0xFF3333, 0xFF0033, 0xCC0033
};

const int SAMPLE_WINDOW = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

const int THRESHOLD = 30;    // Threshold
unsigned int peakToPeak = 0;  // peak-to-peak level

int maxVal = 0;

void setup() {

  strip.begin(); // This initializes the NeoPixel library
  strip.setBrightness(100); // 1/3 brightness

  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 255); // set the color
  }
  strip.show();
  delay(1000);
}

void loop() {

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 ms
  while (countMillis < SAMPLE_WINDOW) {

    sample = analogRead(MIC_PIN);

    if (sample > signalMax) {
      signalMax = sample;  // save just the max levels
    }
    else if (sample < signalMin) {
      signalMin = sample;  // save just the min levels
    }
  }

  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

  peakToPeak = map(peakToPeak, 0, 200, 0, NUMPIXELS); // Diminuer la valeur 200 pour plus de sensibilité
  peakToPeak = constrain(peakToPeak, 0, NUMPIXELS);

  for (int i = 0; i < peakToPeak; i++) {
    strip.setPixelColor(i, myColors[i]); // set the color
  }
  for (int i = peakToPeak; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 0); // set the color
  }
  strip.show();

  countMillis = 0;
}

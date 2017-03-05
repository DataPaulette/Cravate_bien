#include <elapsedMillis.h>
#include <Adafruit_NeoPixel.h>

elapsedMillis countMillis;

const int DATA_PIN = 4;                 // P4 on the Digispark-ATTiny85
const int MIC_PIN = 1;                  // P2 on the Digispark-ATTiny85
const int NUMPIXELS = 20;               // Number of LEDs
const int NUMPIXELS_D2 = NUMPIXELS / 2; // Number of LEDs divide by two

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

uint32_t myColors[NUMPIXELS] = {0};

// From blue to red
uint32_t myColors_A[NUMPIXELS] = {
  0x4038FF, 0x4A35F2, 0x5433E6, 0x5E31D9, 0x682FCD, 0x722DC0, 0x7C2BB4, 0x8628A7, 0x90269B, 0x9A248E,
  0xA42282, 0xAE2075, 0xB81E69, 0xC21B5C, 0xCC1950, 0xD61743, 0xE01537, 0xEA132A, 0xF4111E, 0xFF0F12
};

// All colors
uint32_t myColors_B[NUMPIXELS] = {
  0x3F38FF, 0x3558FF, 0x3382FF, 0x31ACFF, 0x2FD8FF, 0x2DFFF9, 0x2BFFCC, 0x28FF9E, 0x26FF6E, 0x24FF3E,
  0x36FF22, 0x64FF20, 0x92FF1E, 0xC2FF1B, 0xF2FF19, 0xFFDA17, 0xFFA715, 0xFF7413, 0xFF4011, 0xFF0F12
};

const int SAMPLE_WINDOW = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

const int THRESHOLD = 30;     // Threshold
unsigned int peakToPeak = 0;  // peak-to-peak level

int maxVal = 0;

void setup() {

  // memcpy( myColors, myColors_A, NUMPIXELS * sizeof(uint32_t) );
  memcpy( myColors, myColors_B, NUMPIXELS * sizeof(uint32_t) );

  strip.begin(); // This initializes the NeoPixel library
  strip.setBrightness(100); // fuul brightness

  allON(150);
  allOFF();
  allON(150);
  allOFF();
  allON(150);

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
  peakToPeak = map(peakToPeak, 0, 400, 0, NUMPIXELS_D2); // Diminuer la valeur 1024 pour plus de sensibilité
  peakToPeak = constrain(peakToPeak, 0, NUMPIXELS_D2);

  allOFF();

  for (int i = 0; i < peakToPeak; i++) {
    int reight = (i + NUMPIXELS_D2);
    int left = (NUMPIXELS_D2) - i;
    strip.setPixelColor(left, myColors[i]); // set the color
    strip.setPixelColor(reight, myColors[i]); // set the color
  }
  strip.show();

  countMillis = 0;
}

void allOFF() {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 0); // set the color
  }
}

void allON( int DEL) {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, myColors[i]); // set the color
    strip.show();
    delay(DEL);
  }
}

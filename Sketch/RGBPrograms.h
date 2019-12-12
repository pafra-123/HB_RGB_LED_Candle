#ifndef __RGBPROGRAMS_H__
#define __RGBPROGRAMS_H__

void RGBProgramRainbow(uint8_t speed, uint8_t brightness) {
  static uint8_t startIndex = 0;
  static unsigned long lastmillis = millis();
  if (millis() - lastmillis > speed) {
    startIndex = startIndex + 1;
    for ( int i = 0; i < WSNUM_LEDS; i++) {
      leds[i] = ColorFromPalette( RainbowColors_p, startIndex, brightness, LINEARBLEND);
    }
    lastmillis = millis();
  }
}


void RGBProgramFire(uint8_t brightness) {
  static unsigned long lastmillis = millis();
  if (millis() - lastmillis > 15) {

    static byte heat[WSNUM_LEDS];

    for ( int i = 0; i < WSNUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((FIRE_PROGRAM_COOLING * 10) / WSNUM_LEDS) + 2));
    }

    for ( int k = WSNUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    if ( random8() < FIRE_PROGRAM_SPARKLING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160, 255) );
    }

    for ( int j = 0; j < WSNUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;

      pixelnumber = j;

      leds[pixelnumber] = color;
    }
    lastmillis = millis();
  }
}

void RGBProgramWaterfall (uint8_t brightness, uint8_t currentColor)
{
  int brightrange = (brightness / 100) * 80;
  int brightmin = brightness - brightrange;
  int farbe = currentColor;
  
//  int timeout = random(50, 255);
  int timeout = random(brightmin, brightness);
  delay(timeout);

//  int r = random(200, 255);
//  int g = random(150, 200);
//  int b = random(0, 15);

  int flicker = random(20, 100);
//  int flicker = random(brightmin, brightness);

  for(int dot = 0; dot < WSNUM_LEDS; dot++)
  {
    //  FastLED.showColor(CHSV(160, 127, 191)); // replace with RGB code

//    leds[dot] = CRGB(r, g, b);
//    leds[dot] = 0xffb700;
//    leds[dot] = currentColor;
   leds[dot] = CHSV((farbe * 1275L) / 1000, (farbe <  200) ? 255 : 0, 255);

    leds[dot].fadeLightBy( flicker );


//    FastLED.show();
  }


}

void RGBProgramTVSimulation (uint8_t brightness, uint8_t currentColor)
{
  int brightrange = (brightness / 100) * 10;
  int brightmin = brightness - brightrange;
  int farbe = currentColor;
  
//  int timeout = random(50, 255);
  int timeout = random(brightmin, brightness);
  delay(timeout);

//  int r = random(200, 255);
//  int g = random(150, 200);
//  int b = random(0, 15);

  int flicker = random(10, 100);
//  int flicker = random(brightmin, brightness);

  for(int dot = 0; dot < WSNUM_LEDS; dot++)
  {
    //  FastLED.showColor(CHSV(160, 127, 191)); // replace with RGB code

//    leds[dot] = CRGB(r, g, b);
//    leds[dot] = 0xffb700;
//    leds[dot] = currentColor;
   leds[dot] = CHSV((farbe * 1275L) / 1000, (farbe <  200) ? 255 : 0, 255);

    leds[dot].fadeLightBy( flicker );


//    FastLED.show();
  }


}
#endif

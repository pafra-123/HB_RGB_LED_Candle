//- -----------------------------------------------------------------------------------------------------------------------
// 2019-12-12 jp112sdl Creative Commons - http://creativecommons.org/licenses/by-nc-sa/3.0/de/
//- -----------------------------------------------------------------------------------------------------------------------

// define this to read the device id, serial and device type from bootloader section
// #define USE_OTA_BOOTLOADER

#define USE_BATTERY

#ifdef USE_BATTERY
#define USE_WOR
#define BATTSENS BattSensor<AsyncMeter<InternalVCC>>
#define MODEL 0x50
#else
#define BATTSENS NoBattery
#define MODEL 0x51
#endif

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>
#include <FastLED.h>
#include <SPI.h>
#include <AskSinPP.h>
#include <LowPower.h>
#include <Register.h>
#include <Dimmer.h>

#define WSNUM_LEDS    1          //Anzahl angeschlossener LEDs
#define WSLED_PIN     9          //GPIO Pin LED Anschluss
#define WSLED_ENPIN   7          //Enable Pin StepUp
#define WSLED_TYPE    WS2812B    //LED Typ
#define WSCOLOR_ORDER GRB        //Farbreihenfolge


#define SLOW_PROGRAM_TIMER     30     //ms Wartezeit für den Übergang
#define NORMAL_PROGRAM_TIMER   15     //ms Wartezeit für den Übergang
#define FAST_PROGRAM_TIMER     0      //ms Wartezeit für den Übergang
#define FIRE_PROGRAM_COOLING   55
#define FIRE_PROGRAM_SPARKLING 120

#define CONFIG_BUTTON_PIN 8
#define ONBOARD_LED_PIN   4

CRGB leds[WSNUM_LEDS];
#include "RGBPrograms.h"

#define PEERS_PER_CHANNEL 4

using namespace as;

// define all device properties
const struct DeviceInfo PROGMEM devinfo = {
  {0xF3, MODEL, 0x00},    // Device ID
  "JPFLCKLED0",           // Device Serial
  {0xF3, MODEL},          // Device Model
  0x10,                   // Firmware Version
  as::DeviceType::Dimmer, // Device Type
  {0x01, 0x00}            // Info Bytes
};

/**
   Configure the used hardware
*/

typedef AskSin<StatusLed<ONBOARD_LED_PIN>, BATTSENS, Radio<LibSPI<10>, 2>> HalType;

DEFREGISTER(Reg0, MASTERID_REGS, DREG_LOWBATLIMIT)
class Ws28xxList0 : public RegList0<Reg0> {
  public:
    Ws28xxList0(uint16_t addr) : RegList0<Reg0>(addr) {}

    void defaults () {
      clear();
      lowBatLimit(22);
    }
};

// we need no PWM class
class DummyPWM {
public:
  void init(uint8_t __attribute__ ((unused)) pwm) {}
  void set(uint8_t __attribute__ ((unused)) pwm) {}
};

template<class HalType,class DimmerType,class PWM>
class RGBControl : public DimmerControl<HalType,DimmerType,PWM> {
private:
  bool active;
  bool lastactive;
  uint8_t ledenablepin;
public:
  typedef DimmerControl<HalType,DimmerType,PWM> BaseControl;
  RGBControl (DimmerType& dim) : BaseControl(dim), active(false), lastactive(false), ledenablepin(0) {
    for (int i = 0; i < WSNUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.addLeds<WSLED_TYPE, WSLED_PIN, WSCOLOR_ORDER>(leds, WSNUM_LEDS);
    FastLED.setBrightness(255);
  }

  virtual ~RGBControl () {}

  void enableLED(bool en) {
    digitalWrite(ledenablepin, en);
  }

  bool isActive() {
    return active;
  }

  void isActive(bool a) {
    lastactive = active;
    if (lastactive != active) {
      enableLED(active);
    }
    active = a;
  }

  void initLED(uint8_t p) {
    ledenablepin = p;
    pinMode(ledenablepin, OUTPUT); //LED-Kerze - 5V_Enable
  }

  virtual void updatePhysical () {
    // first calculate all physical values of the dimmer channels
    BaseControl::updatePhysical();
    // set brightness and color to LEDs
    uint8_t dimlevel = this->physical[0];
    uint8_t collevel = this->physical[1];
    uint8_t prognum  = this->physical[2];

    isActive(dimlevel > 0);

    switch (prognum) {
    case 0:
      for( uint8_t i=0; i<WSNUM_LEDS; i++) { leds[i] = CHSV((collevel * 1275L) / 1000, (collevel <  200) ? 255 : 0, dimlevel); }
      isActive(false);
      break;
    case 1:
      RGBProgramRainbow(SLOW_PROGRAM_TIMER, dimlevel);
      break;
    case 2:
      RGBProgramRainbow(NORMAL_PROGRAM_TIMER, dimlevel);
      break;
    case 3:
      RGBProgramRainbow(FAST_PROGRAM_TIMER, dimlevel);
      break;
    case 4:
      RGBProgramFire(dimlevel);
      break;
    case 5:
      RGBProgramWaterfall(dimlevel, collevel);
      break;
    case 6:
      RGBProgramTVSimulation(dimlevel, collevel);
      break;
    }

    FastLED.show();
  }
};



class RGBLEDType : public DimmerDevice<HalType,DimmerChannel<HalType, PEERS_PER_CHANNEL, Ws28xxList0>,3,1,Ws28xxList0> {
public:
  typedef DimmerDevice<HalType,DimmerChannel<HalType, PEERS_PER_CHANNEL, Ws28xxList0>,3,1,Ws28xxList0> DevType;
  RGBLEDType (const DeviceInfo& i,uint16_t addr) : DevType(i,addr) {}
  virtual ~RGBLEDType () {}

  virtual void configChanged () {
    DevType::configChanged();
#ifdef USE_BATTERY
    uint8_t lowbat = getList0().lowBatLimit();
    DDECLN(lowbat);
    if( lowbat > 0 ) {
      battery().low(lowbat);
    }
#endif
  }
};

HalType hal;
RGBLEDType sdev(devinfo, 0x20);
RGBControl<HalType,RGBLEDType,DummyPWM> control(sdev);
ConfigButton<RGBLEDType> cfgBtn(sdev);

void setup () {
  DINIT(57600, ASKSIN_PLUS_PLUS_IDENTIFIER);
  uint8_t pins[6] = {0,0,0,0,0,0}; // all 6 PWM pins are 0
  control.init(hal,pins);
  control.initLED(WSLED_ENPIN);
  buttonISR(cfgBtn, CONFIG_BUTTON_PIN);

#ifdef USE_BATTERY
  hal.activity.stayAwake(seconds2ticks(20));
  // measure battery every 4 hours
  hal.battery.init(seconds2ticks(60UL*60*4),sysclock);
#endif

  sdev.initDone();
}

void loop() {
  bool worked = hal.runready();
  bool poll = sdev.pollRadio();
  bool active = control.isActive();
  if ( worked == false && poll == false && active == false) {
#ifdef USE_BATTERY
    hal.activity.savePower<Sleep<>>(hal);
#endif
  }
}

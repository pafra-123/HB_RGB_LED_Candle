//- -----------------------------------------------------------------------------------------------------------------------
// AskSin++
// 2018-08-03 jp112sdl Creative Commons - http://creativecommons.org/licenses/by-nc-sa/3.0/de/
//- -----------------------------------------------------------------------------------------------------------------------

// define this to read the device id, serial and device type from bootloader section
// #define USE_OTA_BOOTLOADER

#define USE_BATTERY

#ifdef USE_BATTERY
#define USE_WOR
#define BAT_CRITICAL 15
#define BATTSENS BattSensor<AsyncMeter<InternalVCC>>
#define MODEL 0x50
#else
#define BATTSENS NoBattery
#define MODEL 0x51
#endif

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>
#include <SPI.h>
#include <AskSinPP.h>
#include <LowPower.h>
#include <Register.h>

#define WSNUM_LEDS    1          //Anzahl angeschlossener LEDs
#define WSLED_PIN     9          //GPIO Pin LED Anschluss 
#define WSLED_ENPIN   5          //Enable Pin StepUp
#define WSLED_TYPE    WS2812B    //LED Typ
#define WSCOLOR_ORDER GRB        //Farbreihenfolge


#define SLOW_PROGRAM_TIMER     30     //ms Wartezeit für den Übergang
#define NORMAL_PROGRAM_TIMER   15     //ms Wartezeit für den Übergang
#define FAST_PROGRAM_TIMER     0      //ms Wartezeit für den Übergang
#define FIRE_PROGRAM_COOLING   55
#define FIRE_PROGRAM_SPARKLING 120


#define CONFIG_BUTTON_PIN 8
#define ONBOARD_LED_PIN   4

#include "RGBCtrl.h"

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

typedef RGBLEDChannel<HalType, PEERS_PER_CHANNEL, Ws28xxList0> ChannelType;

class RGBLEDType : public RGBLEDDevice<HalType, ChannelType, 3, Ws28xxList0> {
private:
    uint8_t wsledpin;
  public:
    typedef RGBLEDDevice<HalType, ChannelType, 3, Ws28xxList0> DevType;
    RGBLEDType (const DeviceInfo& i, uint16_t addr) : DevType(i, addr), wsledpin(0) {}
    virtual ~RGBLEDType () {}

    void initWSLED(uint8_t p) {
      wsledpin = p;
      pinMode(wsledpin, OUTPUT); //LED-Kerze - 5V_Enable
    }

    void enableWSLED(bool b) {
      digitalWrite(wsledpin, b);
    }

    virtual void configChanged () {
      DevType::configChanged();
#ifdef USE_BATTERY
      uint8_t lowbat = getList0().lowBatLimit();
      DDECLN(lowbat);
      if ( lowbat > 0 ) {
        battery().low(lowbat);
      }
#endif
    }
};

HalType hal;
RGBLEDType sdev(devinfo, 0x20);
ConfigButton<RGBLEDType> cfgBtn(sdev);

void setup () {
  DINIT(57600, ASKSIN_PLUS_PLUS_IDENTIFIER);
  sdev.init(hal);
  buttonISR(cfgBtn, CONFIG_BUTTON_PIN);
#ifdef USE_BATTERY
  hal.activity.stayAwake(seconds2ticks(5));
  // measure battery every 4 hours
  hal.battery.init(seconds2ticks(60UL * 60 * 4), sysclock);
  hal.battery.critical(BAT_CRITICAL);
#endif
  sdev.initWSLED(WSLED_ENPIN);
  sdev.enableWSLED(true);
  sdev.initDone();
}

void loop() {
  bool worked = hal.runready();
  bool poll = sdev.pollRadio();
  bool on = (sdev.getCurrentLevel() > 0);

  if (hal.battery.critical())
    hal.activity.sleepForever(hal);

#ifdef USE_BATTERY
  sdev.enableWSLED(on);
  if ( worked == false && poll == false && (sdev.getCurrentProgram() == 0 || on == false)) {
    DPRINT("SLEEP ");DDECLN(millis());
    Serial.flush();
    hal.sleep();
  }
#endif

  sdev.handleLED();
}

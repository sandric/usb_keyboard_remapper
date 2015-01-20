#include <hidboot.h>
#include <usbhub.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include "TimerOne.h"
#include "UsbKeyboard.h"

volatile uint8_t buttonPressed;
volatile uint8_t modPressed;
volatile uint8_t buttonToRepeat;
volatile uint8_t modToRepeat;
volatile uint8_t waitCycleCounter;

int waitTimeToStick = 30000;
int waitCyclesToStick = 12;

class KbdRptParser : 
public KeyboardReportParser
{
  void PrintKey(uint8_t key, uint8_t mod);

protected:
  virtual void OnControlKeysChanged(uint8_t before, uint8_t after);

  virtual void OnKeyDown(uint8_t mod, uint8_t key);
  virtual void OnKeyUp(uint8_t mod, uint8_t key); 
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{ 
  Serial.println("DN");
  PrintKey(key, mod);
 
  Timer1.stop();
  waitCycleCounter = 0;

  parseKeystroke(key, mod);
  
  UsbKeyboard.sendKeyStroke(buttonPressed, modPressed);

  Timer1.restart();
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  Serial.println("UP");
  PrintKey(key, mod);

  Timer1.stop();
  buttonPressed = 0;
  modPressed = 0;
};


void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
    Serial.println("LeftCtrl changed");
  }
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
    Serial.println("LeftShift changed"); 
  }
  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
    Serial.println("LeftAlt changed");
  }
  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
    Serial.println("LeftGUI changed");
  }

  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
    Serial.println("RightCtrl changed");
  }
  if (beforeMod.bmRightShift != afterMod.bmRightShift) {
    Serial.println("RightShift changed");
  }
  if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
    Serial.println("RightAlt changed");
  }
  if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
    Serial.println("RightGUI changed");
  }
};

void KbdRptParser::PrintKey(uint8_t key, uint8_t mod)
{
  Serial.print("key: ");
  Serial.println(key);
  Serial.print("modifier: ");
  Serial.println(mod);
};


void parseKeystroke(uint8_t key, uint8_t mod) {
  uint8_t buttonChanged, modChanged;
  if(mod == 2) {
    switch(key) {
      case 24: buttonChanged = 82; modChanged = 0; break; //LeftShift + U - up arrow
      case 17: buttonChanged = 80; modChanged = 0; break; //LeftShift + N - left arrow
      case 12: buttonChanged = 79; modChanged = 0; break; //LeftShift + I - right arrow
      case 8:  buttonChanged = 81; modChanged = 0; break; //LeftShift + E - down arrow
      
      case 15: buttonChanged = 74; modChanged = 0; break; //LeftShift + L - home
      case 28: buttonChanged = 77; modChanged = 0; break; //LeftShift + Y - end
      
      case 18: buttonChanged = 75; modChanged = 0; break; //leftShift + O - UP
      case 56: buttonChanged = 78; modChanged = 0; break; //leftShift + / - DOWN
      
      case 10: buttonChanged = 47; modChanged = 2; break; //leftShift + G - {
      case 13: buttonChanged = 48; modChanged = 2; break; //leftShift + J - }
      case 7:  buttonChanged = 38; modChanged = 2; break; //leftShift + D - (
      case 11: buttonChanged = 39; modChanged = 2; break; //leftShift + H - )
      case 5:  buttonChanged = 47; modChanged = 0; break; //leftShift + B - [
      case 14: buttonChanged = 48; modChanged = 0; break; //leftShift + K - ]
      
      case 16: buttonChanged = 56; modChanged = 0; break; //leftShift + V - /
      case 25: buttonChanged = 49; modChanged = 0; break; //leftShift + M - 
      
      case 19: buttonChanged = 45; modChanged = 2; break; //leftShift + P - _
      case 23: buttonChanged = 46; modChanged = 0; break; //leftShift + T - =

      
      case 29: buttonChanged = 30; modChanged = 0; break; //1
      case 27: buttonChanged = 31; modChanged = 0; break; //2
      case 6:  buttonChanged = 32; modChanged = 0; break; //3

      case 4:  buttonChanged = 33; modChanged = 0; break; //4
      case 21: buttonChanged = 34; modChanged = 0; break; //5
      case 22: buttonChanged = 35; modChanged = 0; break; //6
      
      case 20: buttonChanged = 36; modChanged = 0; break; //7
      case 26: buttonChanged = 37; modChanged = 0; break; //8
      case 9:  buttonChanged = 38; modChanged = 0; break; //9
    }
  }
  if(mod == 4) {
    switch(key) {
      case 43: buttonChanged = 54; modChanged = 0; break; //Shuffle - ,
    }
  }
  if(mod == 8) {
    switch(key) {
      case 7: buttonChanged = 55; modChanged = 0; break; //Shuffle - .
    }
  }
  if(mod == 0) {
    switch(key){
      case 51: buttonChanged = 51; modChanged = 2; break; //; - :
      case 47: buttonChanged = 51; modChanged = 0; break; //{ - ;
      
      case 54: buttonChanged = 46; modChanged = 2; break; //plus
      case 55: buttonChanged = 45; modChanged = 0; break; //minus

      case 45: buttonChanged = 39; modChanged = 0; break; //zero
      
      case 30: buttonChanged = 30; modChanged = 2; break; //!
      case 31: buttonChanged = 35; modChanged = 2; break; //^
      case 32: buttonChanged = 33; modChanged = 2; break; //$
      case 33: buttonChanged = 37; modChanged = 2; break; //*
      case 34: buttonChanged = 36; modChanged = 2; break; //&
      case 35: buttonChanged = 49; modChanged = 2; break; //|
      case 36: buttonChanged = 32; modChanged = 2; break; //#
      case 37: buttonChanged = 31; modChanged = 2; break; //@
      case 38: buttonChanged = 34; modChanged = 2; break; //%
      case 39: buttonChanged = 56; modChanged = 2; break; //?
    }
  }
  if(buttonChanged) {
    buttonPressed = buttonChanged;
    modPressed = modChanged;
  } else {
    buttonPressed = key;
    modPressed = mod;
  }
}

void interrupt(void) {
  if(waitCycleCounter < waitCyclesToStick) {
    waitCycleCounter++;
  } else if(buttonPressed != 0) {    
    buttonToRepeat = buttonPressed;
    modToRepeat = modPressed;
  }
}

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

uint32_t next_time;

KbdRptParser Prs;

void setup()
{
  Serial.begin( 115200 );
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  next_time = millis() + 5000;

  HidKeyboard.SetReportParser(0, (HIDReportParser*)&Prs);

  cli();

  // Force re-enumeration so the host will detect us
  usbDeviceDisconnect();
  delayMs(250);
  usbDeviceConnect();

  // Set interrupts again
  sei();

  Timer1.initialize(waitTimeToStick);
  Timer1.attachInterrupt(interrupt);
}

void loop()
{
  Usb.Task();

  UsbKeyboard.update();

  if(buttonToRepeat){
    UsbKeyboard.sendKeyStroke(buttonToRepeat, modToRepeat);
    buttonToRepeat = 0;
    modToRepeat = 0;
  }
}

// helper method for V-USB library
void delayMs(unsigned int ms) {
  for( int i=0; i<ms; i++ ) {
    delayMicroseconds(1000);
  }
}



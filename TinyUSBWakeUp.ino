#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
void setup() {}
void loop() {}
#else

// Using Adafruit TinyUSB. Currently only version 3.1.1 and lower work due to a bug
#include "Adafruit_TinyUSB.h"

// HID report descriptor using TinyUSB's template
// Single Report (no ID) descriptor
uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_KEYBOARD()
};

// USB HID object. For ESP32 these values cannot be changed after this declaration
// desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_hid;

// BUTTON_PIN: Button to trigger wake up
#define BUTTON_PIN 4
// LED_PIN: Led to indicate wake up command is beeing processed
#define LED_PIN 5


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // Init USB device
  TinyUSB_Device_Init(0);

  // Setup HID
  usb_hid.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("TinyUSB Keyboard");
  usb_hid.begin();


  // wait until device mounted
  while (!TinyUSBDevice.mounted()) delay(1);
}

void loop() {
  byte buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    // When button is pressed, light up led and send wake up signal
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    TinyUSBDevice.remoteWakeup();
  }
  delay(100);
}

#endif /* ARDUINO_USB_MODE */

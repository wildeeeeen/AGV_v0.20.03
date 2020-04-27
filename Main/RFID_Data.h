#include <usbhid.h>
#include <usbhub.h>
#include <hiduniversal.h>
#include <hidboot.h>
#include <SPI.h>


String scanResult;
String scanResult2;
bool scanFinished;

class MyParser : public HIDReportParser {
  public:
    MyParser();
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
  protected:
    uint8_t KeyToAscii(bool upper, uint8_t mod, uint8_t key);
    virtual void OnKeyScanned(bool upper, uint8_t mod, uint8_t key);
    virtual void OnScanFinished();
};

MyParser::MyParser() {}
USB          Usb;
HIDUniversal Hid(&Usb);
MyParser     Parser;

void MyParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
  if (buf[2] == 1 || buf[2] == 0) return;   // If error or empty, return
  for (uint8_t i = 7; i >= 2; i--) {
    if (buf[i] == 0) continue;              // If empty, skip
    if (buf[i] == UHS_HID_BOOT_KEY_ENTER) { // If enter signal emitted, scan finished
      OnScanFinished();
    }
    else {                                  // If not, continue normally
      OnKeyScanned(i > 2, buf, buf[i]);     // If bit position not in 2, it's uppercase words
    }
    return;
  }
}

uint8_t MyParser::KeyToAscii(bool upper, uint8_t mod, uint8_t key) {
  if (VALUE_WITHIN(key, 0x04, 0x1d)) {      // Letters
    if (upper) return (key - 4 + 'A');
    else return (key - 4 + 'a');
  }
  else if (VALUE_WITHIN(key, 0x1e, 0x27)) { // Numbers
    return ((key == UHS_HID_BOOT_KEY_ZERO) ? '0' : key - 0x1e + '1');
  }
  return 0;
}

void MyParser::OnKeyScanned(bool upper, uint8_t mod, uint8_t key) {
  uint8_t ascii = KeyToAscii(upper, mod, key);
  scanResult = scanResult + (char)ascii;
}

void MyParser::OnScanFinished() {
  scanResult = scanResult.substring(scanResult.length() - 5, scanResult.length());    // ini hasil dari RFID HID Reader
  scanFinished = true;
//  Serial.println(scanResult);
}

void setup_RFID_Data(){
  Serial.println("Start RFID Reader");
  if (Usb.Init() == -1) {
    Serial.println("OSC did not start.");
  }
  delay(200);
  Hid.SetReportParser(0, &Parser);
}

// Code to emulate a 3DConnexion Space Mouse by jfedor: https://pastebin.com/gQxUrScV

// Include inbuilt Arduino HID library by NicoHood: https://github.com/NicoHood/HID 
#include "HID.h"

// Debugging
// 0: Debugging off. Set to this once everything is working.
// 1: Output raw joystick values. 0-1023 raw ADC 10-bit values
// 2: Output centered joystick values. Values should be approx -500 to +500, jitter around 0 at idle.
// 3: Output centered joystick values. Filtered for deadzone. Approx -500 to +500, locked to zero at idle.
// 4: Output translation and rotation values. Approx -800 to 800 depending on the parameter.
// 5: Output debug 4 and 5 side by side for direct cause and effect reference.
int debug = 0;

// Deadzone to filter out unintended movements. Increase if the mouse has small movements when it should be idle or the mouse is too senstive to subtle movements.
int DEADZONE = 40;
// Speed
// Modify to change sensitibity/speed. Default and maximum 100. Works like a percentage ie. 50 is half as fast as default. This can also be done per application in the 3DConnexion software.
int16_t speed = 80;

// Direction
// Modify the direction of translation/rotation depending on preference. This can also be done per application in the 3DConnexion software.
// Switch between true/false as desired.
bool invX = false; // pan left/right
bool invY = false; // pan up/down
bool invZ = false; // zoom in/out
bool invRX = false; // Rotate around X axis (tilt front/back)
bool invRY = true; // Rotate around Y axis (tilt left/right)
bool invRZ = true; // Rotate around Z axis (twist left/right)

// Default Assembly when looking from above:
//   USB-C
//    C           Y+
//    |           .
// B--+--D   X-...Z+...X+
//    |           .
//    A           Y-
//
// Wiring. Matches the first eight analogue pins of the Arduino Pro Micro (atmega32u4)
int PINLIST[8] = { // The positions of the reads
  A8, // X-axis A
  A9, // Y-axis A
  A3, // X-axis B
  A10,// Y-axis B
  A0, // X-axis C
  A1, // Y-axis C
  A7, // X-axis D
  A6  // Y-axis D
};

// This portion sets up the communication with the 3DConnexion software. The communication protocol is created here.
// hidReportDescriptor webpage can be found here: https://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/ 
static const uint8_t _hidReportDescriptor[] PROGMEM = {
  0x05, 0x01,           //  Usage Page (Generic Desktop)
  0x09, 0x08,           //  0x08: Usage (Multi-Axis)
  0xa1, 0x01,           //  Collection (Application)
  0xa1, 0x00,           // Collection (Physical)
  0x85, 0x01,           //  Report ID
  0x16, 0x00, 0x80,     //logical minimum (-500)
  0x26, 0xff, 0x7f,     //logical maximum (500)
  0x36, 0x00, 0x80,     //Physical Minimum (-32768)
  0x46, 0xff, 0x7f,     //Physical Maximum (32767)
  0x09, 0x30,           //    Usage (X)
  0x09, 0x31,           //    Usage (Y)
  0x09, 0x32,           //    Usage (Z)
  0x75, 0x10,           //    Report Size (16)
  0x95, 0x03,           //    Report Count (3)
  0x81, 0x02,           //    Input (variable,absolute)
  0xC0,                 //  End Collection
  0xa1, 0x00,           // Collection (Physical)
  0x85, 0x02,           //  Report ID
  0x16, 0x00, 0x80,     //logical minimum (-500)
  0x26, 0xff, 0x7f,     //logical maximum (500)
  0x36, 0x00, 0x80,     //Physical Minimum (-32768)
  0x46, 0xff, 0x7f,     //Physical Maximum (32767)
  0x09, 0x33,           //    Usage (RX)
  0x09, 0x34,           //    Usage (RY)
  0x09, 0x35,           //    Usage (RZ)
  0x75, 0x10,           //    Report Size (16)
  0x95, 0x03,           //    Report Count (3)
  0x81, 0x02,           //    Input (variable,absolute)
  0xC0,                 //  End Collection
 
  0xa1, 0x00,           // Collection (Physical)
  0x85, 0x03,           //  Report ID
  0x15, 0x00,           //   Logical Minimum (0)
  0x25, 0x01,           //    Logical Maximum (1)
  0x75, 0x01,           //    Report Size (1)
  0x95, 32,             //    Report Count (24)
  0x05, 0x09,           //    Usage Page (Button)
  0x19, 1,              //    Usage Minimum (Button #1)
  0x29, 32,             //    Usage Maximum (Button #24)
  0x81, 0x02,           //    Input (variable,absolute)
  0xC0,
  0xC0
};

// Axes are matched to pin order.
#define AX 0
#define AY 1
#define BX 2
#define BY 3
#define CX 4
#define CY 5
#define DX 6
#define DY 7

//Declaration of arrays to be populated during setup and runtime routines:
int centerPoints[8], rawReads[8], centered[8]; //moved here from loop declaration to avoid memory overhead

//Final movement variables:
int16_t transX, transY, transZ, rotX, rotY, rotZ; // Declare movement variables at 16 bit integers

// Function to read and store analogue voltages for each joystick axis.
void readAllFromJoystick(int *rawReads){
  for(int i=0; i<8; i++){
    rawReads[i] = analogRead(PINLIST[i]);
  }
}

void setup() {
  // HID protocol is set.
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);
  
  if (debug != 0) { // Begin Seral for debugging
    Serial.begin(250000);
    delay(100);
  }

  // Read idle/centre positions for joysticks.
  readAllFromJoystick(centerPoints);
}

// Function to send translation and rotation data to the 3DConnexion software using the HID protocol outlined earlier. Two sets of data are sent: translation and then rotation.
// For each, a 16bit integer is split into two using bit shifting. The first is mangitude and the second is direction.
void send_command(int16_t rx, int16_t ry, int16_t rz, int16_t x, int16_t y, int16_t z) {
  uint8_t trans[6] = { x & 0xFF, x >> 8, y & 0xFF, y >> 8, z & 0xFF, z >> 8 };
  HID().SendReport(1, trans, 6);
  uint8_t rot[6] = { rx & 0xFF, rx >> 8, ry & 0xFF, ry >> 8, rz & 0xFF, rz >> 8 };
  HID().SendReport(2, rot, 6);
}

void loop() {
  // Joystick values are read. 0-1023
  readAllFromJoystick(rawReads);
  // Report back 0-1023 raw ADC 10-bit values if enabled
  if(debug == 1){ 
    print_joystick_values(rawReads);
    Serial.print("\n"); //manually go to the next line
  }

  // Subtract centre position from measured position to determine movement.
  for(int i=0; i<8; i++) centered[i] = rawReads[i] - centerPoints[i]; // 
  // Report centered joystick values if enabled. Values should be approx -500 to +500, jitter around 0 at idle.
  if(debug == 2){
    print_joystick_values(centered);
    Serial.print("\n"); //manually go to the next line
  }
  // Filter movement values. Set to zero if movement is below deadzone threshold.
  for(int i=0; i<8; i++){
    if(centered[i]<DEADZONE && centered[i]>-DEADZONE) centered[i] = 0;
  }
  // Report centered joystick values. Filtered for deadzone. Approx -500 to +500, locked to zero at idle
  if(debug == 3){
    print_joystick_values(centered);
    Serial.print("\n"); //manually go to the next line
  }

  // Doing all through arithmetic contribution by fdmakara
  // Original fdmakara calculations
  //transX = (-centered[AX] +centered[CX])/1;
  //transY = (-centered[BX] +centered[DX])/1;
  //transZ = (-centered[AY] -centered[BY] -centered[CY] -centered[DY])/2;
  //rotX = (-centered[AY] +centered[CY])/2;
  //rotY = (+centered[BY] -centered[DY])/2;
  //rotZ = (+centered[AX] +centered[BX] +centered[CX] +centered[DX])/4;
  // My altered calculations based on debug output. Final divisor can be changed to alter sensitivity for each axis.
  transX = (centered[CX] - centered[AX])/2;  
  transY = (centered[DX] - centered[BX])/2;  
  transZ = -(centered[AY] + centered[BY] + centered[CY] + centered[DY])/4;
  rotX = (centered[CY] - centered[AY])/2;
  rotY = (centered[BY] - centered[DY])/2;
  rotZ = -(centered[AX] +centered[BX] +centered[CX] +centered[DX])/4;

// Alter speed to suit user preference
  transX = (transX * speed)/100;
  transY = (transY * speed)/100;
  transZ = (transZ * speed)/100;
  rotX = (rotX * speed)/100;
  rotY = (rotY * speed)/100;
  rotZ = (rotZ * speed)/100;
// Invert directions if needed
  if(invX == true){ transX = transX*-1;};
  if(invY == true){ transY = transY*-1;};
  if(invZ == true){ transZ = transZ*-1;};
  if(invRX == true){ rotX = rotX*-1;};
  if(invRY == true){ rotY = rotY*-1;};
  if(invRZ == true){ rotZ = rotZ*-1;};

// Report translation and rotation values if enabled. Approx -800 to 800 depending on the parameter.
  if(debug == 4){
    print_rotations(transX, transY, transZ, rotX, rotY, rotZ);
    Serial.print("\n"); //manually go to the next line
  }
// Report debug 4 and 5 info side by side for direct reference if enabled. Very useful if you need to alter which inputs are used in th arithmatic above.
  if(debug == 5){
    print_joystick_values(centered);
    Serial.print("||");
    print_rotations(transX, transY, transZ, rotX, rotY, rotZ);
    Serial.print("\n"); //manually go to the next line
  }

// Send data to the 3DConnexion software.
// The correct order for me was determined after trial and error
  send_command(rotX, rotY, rotZ, transX, transY, transZ);
}

// HELPER FUNCTIONS:
void print_joystick_values(int *array) {
  Serial.print("AX:");
  Serial.print(array[0]);
  Serial.print(",");
  Serial.print("AY:");
  Serial.print(array[1]);
  Serial.print(",");
  Serial.print("BX:");
  Serial.print(array[2]);
  Serial.print(",");
  Serial.print("BY:");
  Serial.print(array[3]);
  Serial.print(",");
  Serial.print("CX:");
  Serial.print(array[4]);
  Serial.print(",");
  Serial.print("CY:");
  Serial.print(array[5]);
  Serial.print(",");
  Serial.print("DX:");
  Serial.print(array[6]);
  Serial.print(",");
  Serial.print("DY:");
  Serial.print(array[7]);
}

void print_rotations(int16_t TX, int16_t TY, int16_t TZ, int16_t RX, int16_t RY, int16_t RZ) {
  Serial.print("TX:");
  Serial.print(TX);
  Serial.print(",");
  Serial.print("TY:");
  Serial.print(TY);
  Serial.print(",");
  Serial.print("TZ:");
  Serial.print(TZ);
  Serial.print(",");
  Serial.print("RX:");
  Serial.print(RX);
  Serial.print(",");
  Serial.print("RY:");
  Serial.print(RY);
  Serial.print(",");
  Serial.print("RZ:");
  Serial.print(RZ);
}

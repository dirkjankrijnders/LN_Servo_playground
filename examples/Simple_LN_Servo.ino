#define _useTimer3
#include <LocoNet.h>
#include <Servo.h>
#include "../configuredpins.h"

#define MAX 5
int pos = 0;
int incomingByte = 0;

lnMsg        *LnPacket;

uint8_t servo_pins[] = {9,10,16,14,15};
uint8_t addresses[]   = {1,2,3,4,5};
uint16_t maxs[] = {1700, 1600, 1720, 1280, 1920}; // Turnout
uint16_t mins[] = {1350, 1080, 1280, 1720, 1450}; // Straight

ServoSwitch* switches[MAX];

void setup() {
  // put your setup code here, to run once:
  for (unsigned int i = 0; i < MAX; i++) {
	switches[i] = new ServoSwitch(i, servo_pins[i], addresses[i], maxs[i], mins[i], 1, 0, 0, 0);
  }
  
  LocoNet.init(2); // TX on pin 2  

    // Configure the serial port for 57600 baud
  Serial.begin(57600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("Switch decoder ready");
}

void loop() {

   LnPacket = LocoNet.receive() ;
  //Serial.print('.');
  if( LnPacket ) {
      // First print out the packet in HEX
    Serial.print("RX: ");
    uint8_t msgLen = getLnMsgSize(LnPacket); 
    for (uint8_t x = 0; x < msgLen; x++)
    {
      uint8_t val = LnPacket->data[x];
        // Print a leading 0 if less than 16 to make 2 HEX digits
      if(val < 16)
        Serial.print('0');
        
      Serial.print(val, HEX);
      Serial.print(' ');
    }
    
      // If this packet was not a Switch or Sensor Message then print a new line 
    uint8_t packetConsumed(LocoNet.processSwitchSensorMessage(LnPacket));
  }
  
}

void notifySwitchRequest( uint16_t Address, uint8_t Output, uint8_t Direction ) {
  for (uint8_t i =0 ; i < MAX ; i++) {
    if (switches[i]->_address == Address){
      switches[i]->set(Direction, Output);
    }
  }
  Serial.print("Switch Request: ");
  Serial.print(Address, DEC);
  Serial.print(':');
  Serial.print(Direction ? "Closed" : "Thrown");
  Serial.print(" - ");
  Serial.println(Output ? "On" : "Off");
}

#define _useTimer3
#include <LocoNet.h>
#include <Servo.h>


#define LNCV_COUNT 16

// Item Number (Art.-Nr.): 50010
#define ARTNR 5001

//uint16_t moduleAddr;
uint16_t lncv[LNCV_COUNT];


LocoNetCVClass lnCV;

boolean programmingMode;

#define MAX 5
int pos = 0;
int incomingByte = 0;

lnMsg        *LnPacket;

uint8_t servo_pins[] = {9,10,16,14,15};
uint8_t addresses[]   = {1,2,3,4,5};
uint16_t maxs[] = {1700, 1600, 1720, 1280, 1920}; // Turnout
uint16_t mins[] = {1350, 1080, 1280, 1720, 1450}; // Straight

class ServoSwitch {
  public:
    ServoSwitch() {};
    ServoSwitch(uint8_t pin) { _pin = pin; _servo.attach(pin);};
    void changepin(uint8_t pin) {_servo.detach(); _pin = pin; _servo.attach(pin);};
    void setDirection(bool dir) {
      if (dir){
        _servo.writeMicroseconds(_turnout);
      } else {
        _servo.writeMicroseconds(_straight);
      }
      LocoNet.reportSwitch(addresses[1]);
      state = dir;
    }
    void toggle() {
      if (state) {
        _servo.writeMicroseconds(_straight);
        state = !state;
      } else {
        _servo.writeMicroseconds(_turnout);
        state = !state;
      }
    }
    uint16_t _turnout;
    uint16_t _straight;
    bool state;
  private:
    uint8_t _pin;
    Servo _servo;
};

ServoSwitch switches[MAX];

void setup() {
  // put your setup code here, to run once:
  for (unsigned int i = 0; i < MAX; i++) {
    switches[i].changepin(servo_pins[i]);
    switches[i]._turnout = maxs[i];
    switches[i]._straight = mins[i];
    switches[i].setDirection(0);
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
    if (packetConsumed == 0) {
      Serial.print("Loop ");
      Serial.print((int)LnPacket);
      dumpPacket(LnPacket->ub);
      packetConsumed = lnCV.processLNCVMessage(LnPacket);
      Serial.print("End Loop\n");
    } else {
      Serial.println();
    }
    
  }
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    uint8_t add = Serial.parseInt();
    if (add > 0){
      switches[add-1].toggle();
    }
    Serial.print(add );
    Serial.print(": ");
    if (switches[add-1].state) {
      Serial.println("Turnout");
    }else {
      Serial.println("Straight");
    }
  }
}

void dumpPacket(UhlenbrockMsg & ub) {
  Serial.print(" PKT: ");
  Serial.print(ub.command, HEX);
  Serial.print(" ");
  Serial.print(ub.mesg_size, HEX);
  Serial.print(" ");
  Serial.print(ub.SRC, HEX);
  Serial.print(" ");
  Serial.print(ub.DSTL, HEX);
  Serial.print(" ");
  Serial.print(ub.DSTH, HEX);
  Serial.print(" ");
  Serial.print(ub.ReqId, HEX);
  Serial.print(" ");
  Serial.print(ub.PXCT1, HEX);
  Serial.print(" ");
  for (int i(0); i < 8; ++i) {
    Serial.print(ub.payload.D[i], HEX);
    Serial.print(" ");
  }
  Serial.write("\n");
}
void notifySwitchRequest( uint16_t Address, uint8_t Output, uint8_t Direction ) {
  if (Address == 1){
    switches[0].setDirection(Direction);
  }
  for (uint8_t i =0 ; i < MAX ; i++) {
    if (addresses[i] == Address){
      switches[i].setDirection(Direction);
    }
  }
  Serial.print("Switch Request: ");
  Serial.print(Address, DEC);
  Serial.print(':');
  Serial.print(Direction ? "Closed" : "Thrown");
  Serial.print(" - ");
  Serial.println(Output ? "On" : "Off");
}

/**
   * Notifies the code on the reception of a read request.
   * Note that without application knowledge (i.e., art.-nr., module address
   * and "Programming Mode" state), it is not possible to distinguish
   * a read request from a programming start request message.
   */
int8_t notifyLNCVread(uint16_t ArtNr, uint16_t lncvAddress, uint16_t,
    uint16_t & lncvValue) {
  Serial.print("Enter notifyLNCVread(");
  Serial.print(ArtNr, HEX);
  Serial.print(", ");
  Serial.print(lncvAddress, HEX);
  Serial.print(", ");
  Serial.print(", ");
  Serial.print(lncvValue, HEX);
  Serial.print(")");
  // Step 1: Can this be addressed to me?
  // All ReadRequests contain the ARTNR. For starting programming, we do not accept the broadcast address.
  if (programmingMode) {
    if (ArtNr == ARTNR) {
      if (lncvAddress < 16) {
        lncvValue = lncv[lncvAddress];
        Serial.print(" LNCV Value: ");
        Serial.print(lncvValue);
        Serial.print("\n");
        return LNCV_LACK_OK;
      } else {
        // Invalid LNCV address, request a NAXK
        return LNCV_LACK_ERROR_UNSUPPORTED;
      }
    } else {
      Serial.print("ArtNr invalid.\n");
      return -1;
    }
  } else {
    Serial.print("Ignoring Request.\n");
    return -1;
  }
}

int8_t notifyLNCVprogrammingStart(uint16_t & ArtNr, uint16_t & ModuleAddress) {
  // Enter programming mode. If we already are in programming mode,
  // we simply send a response and nothing else happens.
  Serial.print("notifyLNCVProgrammingStart ");
  if (ArtNr == ARTNR) {
    Serial.print("artnrOK ");
    if (ModuleAddress == lncv[0]) {
      Serial.print("moduleUNI ENTERING PROGRAMMING MODE\n");
      programmingMode = true;
      return LNCV_LACK_OK;
    } else if (ModuleAddress == 0xFFFF) {
      Serial.print("moduleBC ENTERING PROGRAMMING MODE\n");
      ModuleAddress = lncv[0];
      return LNCV_LACK_OK;
    }
  }
  Serial.print("Ignoring Request.\n");
  return -1;
}

  /**
   * Notifies the code on the reception of a write request
   */
int8_t notifyLNCVwrite(uint16_t ArtNr, uint16_t lncvAddress,
    uint16_t lncvValue) {
  Serial.print("notifyLNCVwrite, ");
  //  dumpPacket(ub);
  if (!programmingMode) {
    Serial.print("not in Programming Mode.\n");
    return -1;
  }

  if (ArtNr == ARTNR) {
    Serial.print("Artnr OK, ");

    if (lncvAddress < 16) {
      lncv[lncvAddress] = lncvValue;
      return LNCV_LACK_OK;
    }
    else {
      return LNCV_LACK_ERROR_UNSUPPORTED;
    }

  }
  else {
    Serial.print("Artnr Invalid.\n");
    return -1;
  }
}

void commitLNCVUpdate() {
  Serial.print("Module Address is now: ");
  Serial.print(lncv[0]);
  Serial.print("\n");
}
  /**
   * Notifies the code on the reception of a request to end programming mode
   */
void notifyLNCVprogrammingStop(uint16_t ArtNr, uint16_t ModuleAddress) {
  Serial.print("notifyLNCVprogrammingStop ");
  if (programmingMode) {
    if (ArtNr == ARTNR && ModuleAddress == lncv[0]) {
      programmingMode = false;
      Serial.print("End Programing Mode.\n");
      commitLNCVUpdate();
    }
    else {
      if (ArtNr != ARTNR) {
        Serial.print("Wrong Artnr.\n");
        return;
      }
      if (ModuleAddress != lncv[0]) {
        Serial.print("Wrong Module Address.\n");
        return;
      }
    }
  }
  else {
    Serial.print("Ignoring Request.\n");
  }
}

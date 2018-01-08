/*
  SmaToModbus
  Version: 2017-10-03
  Author: Andreas Schwotzer

  This sketch reads information from an SMA-Energy-Meter and provides it via Modbus.

  To define the Modbus-Port, please use the "MB_PORT"-Definition in "MgsModbus.h".
  This sketch uses few modbus-registers. When you plan more, please check the "MbDataLen"-Definition in "MgsModbus.h".

  Arduino
  - Arduino Ethernet
  - Arduino USB2SERIAL (only for transfer sketch)

  This sketch based on:
   - UDPSendReceiveString:  by Michael Margolis (21 Aug 2010)
   - MsgModubus_test_Slave: by Marco Gerritse 2013
   - SMA-EnergyMeter

*/


#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h> // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include "MgsModbus.h"

MgsModbus Mb;


//#define PRINT_VALUES
//#define DEBUG


#define IP_STATIC

#define BUFFER_SIZE 600
#define PORT_MULTI 9522 // See Description of SMA-EnergyMeter

#define POS_170 32
#define LEN_170 4
#define POS_180 52
#define LEN_180 4
#define POS_270 40
#define LEN_270 8
#define POS_280 60
#define LEN_280 8

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x64, 0xF0 };

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;


void setup() {
    // Multicast Adress of the SMAEnergymeter
    IPAddress ipMulti(239, 12, 255, 254);


#ifdef IP_STATIC
  IPAddress gateway(192, 168, 178, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress ip(192, 168, 178, 3);
  
  // start the Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
#else
  Ethernet.begin(mac);
#endif

    // start the UDP
    Udp.beginMulticast(ipMulti, PORT_MULTI);
    Serial.begin(9600);

  // Fill MbData
  // Mb.MbData[0] - 1.7.0 - PowerIn (Watt)
  // Mb.MbData[1] - 1.8.0 - PowerOut (Watt)
  for (int i = 0; i < MbDataLen; i++) {
    Mb.MbData[i] = 0;
  }
}


void loop() {

  // buffers for receiving data
  byte packetBuffer[BUFFER_SIZE];

  int packetSize = 0; // The real size of the packet
  int count = 0; // Counter for the errors while recive a packet
  float energyValue = 0; // Temporary

  //packetSize = Udp.available();
  packetSize = Udp.parsePacket();


  // try to recive packet from Udp max. 10 times
  // and wait 200 millisec. between
  while ((packetSize <= 0) && (count < 10)) {
    count++;
    packetSize = Udp.parsePacket();
    delay(200);

#ifdef DEBUG
    Serial.print("*");
#endif
  }

  // There is an packet
  if (packetSize) {

    // read the packet into packetBuffer
    if (packetSize > BUFFER_SIZE) {
      Udp.read(packetBuffer, BUFFER_SIZE);
    } else {
      Udp.read(packetBuffer, packetSize);
    }

#ifdef DEBUG
    Serial.println("");
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    Serial.print(Udp.remoteIP());
    Serial.print(", port ");
    Serial.println(Udp.remotePort());
    Serial.println("Contents:");
    /* Only whe necessary
      for (int i = 0; i < BUFFER_SIZE ; i++) {
      Serial.print(packetBuffer[i]);
      Serial.print(" ");
      }
      Serial.println("");
    */
#endif


    // Read "PowerIn" from SMA-Telegram
    energyValue = 0;
    for (count = 0; count < LEN_170; count++) {
      energyValue = (energyValue * 256) + packetBuffer[(POS_170 + count)];
    }
    Mb.MbData[0] = (energyValue / 10);

    // Read "PowerOut" from SMA-Telegram
    energyValue = 0;
    for (count = 0; count < LEN_180; count++) {
      energyValue = (energyValue * 256) + packetBuffer[(POS_180 + count)];
    }
    Mb.MbData[1] = (energyValue / 10);

    /* 
     * I didn't need EnergyIn and EnergyOut Values
     * 
        // EnergyIn
        energyValue = 0;
        for (count = 0; count < LEN_270; count++) {
          energyValue = (energyValue * 256) + packetBuffer[(POS_270 + count)];
        }

        // EnergyOut
        energyValue = 0;
        for (count = 0; count < LEN_280; count++) {
          energyValue = (energyValue * 256) + packetBuffer[(POS_280 + count)];
        }
    */

    Mb.MbsRun();

#ifdef PRINT_VALUES
    Serial.print(Mb.MbData[0]);
    Serial.print(" ");
    Serial.println(Mb.MbData[1]);

#endif

    // Routine was successfull
    // At this point, there could be a watchdog-Reset


    // Read and provide the data every 2 Seconds is enought
    delay(2000);

  }



}



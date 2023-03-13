#include <SPI.h> 
#include <LoRa.h> // include libraries
#define SCK 5 // GPIOS --SX127x's SCK
#define MISO 19 // GPIO19-SX127x's MISO GP1027-SX127x's MOSI
#define MOSI 27
#define SS 18 
#define RST 14 
#define DI0 26
#define freq 8695E5
#define sf 10
#define sb 40E3


union pack 
{
  uint8_t frame[16]; // trames avec octets
  float data[4]; // 4 valeurs en virgule flottante
} rdp ; // paquet d'émission


void setup() {
  Serial.begin(115200);
  pinMode (DI0, INPUT); // signal interrupt 
  SPI.begin(SCK, MISO, MOSI, SS); 
  LoRa.setPins (SS, RST, DI0);
  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor (sf);
  LoRa.setSignalBandwidth (sb);
}

float d1,d2;
int i=0;

void loop() 
{
int packetLen;
packetLen=LoRa.parsePacket();
   i=0;
   Serial.println("Listening");
   while (LoRa.available()) {
    rdp.data[i]=LoRa.read();i++;
    }
   d1=rdp.data[0];d2=rdp.data[1];
   int rssi=LoRa.packetRssi();
   Serial.println(d1); Serial.println(d2);
   Serial.println(rssi);
}

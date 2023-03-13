#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCK   5
#define MISO  19
#define MOSI  27
#define SS    18
#define RST   14
#define DI0   26

const char* ssid = "Oscar";
const char* password = "ouiouioui";
const char* mqtt_server = "test.mosquitto.org";
int verif = 0;

float d1; 
float d2;
int y=0; 
long int freq; 
long int sf; 
long int sb;

WiFiClient espClient;
PubSubClient client(espClient);

union pack {
    uint8_t frame[16];
    float data[4]; 
} rdp; 

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(DI0, INPUT);
  SPI.begin(SCK,MISO,MOSI,SS); 
  LoRa.setPins(SS,RST,DI0);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("srt/FJ");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  char* token;
  char* data_lora[3] = {NULL};
  int token_count = 0;

  char payload_str[length + 1];
  for (int i = 0; i < length; i++) {
    payload_str[i] = (char)payload[i];
  }
  payload_str[length] = '\0';

  //Lecture conntinue des données
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(payload_str);

  // Découpage de la chaîne de caractères en tokens en utilisant l'espace comme délimiteur
  if (verif == 0){
    token = strtok(payload_str, " ");
    while (token != NULL) {
      if (token_count >= 3) {
        // Si nous avons déjà récupéré les trois derniers tokens, sortir de la boucle
        break;
      }
      // Stockage des trois derniers tokens dans last_three_tokens
      Serial.println(token);
      data_lora[token_count] = token;
      token_count++;
      token = strtok(NULL, " ");
    }
  freq = strtol(data_lora[0], NULL, 10);
  //Serial.println(freq);
  sf = strtol(data_lora[1], NULL, 10);
  //Serial.println(sf);
  sb = strtol(data_lora[2], NULL, 10);
  //Serial.println(sb);
  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa failed!");
  }
  LoRa.setSpreadingFactor(sf);
  LoRa.setSignalBandwidth(sb);

  verif = 1;
  //Serial.printf("Les trois dernières valeurs sont : %s %s %s\n", data_lora[0], data_lora[1], data_lora[2]);
  }
}

void loop() {
  if (!client.connected()) {
    Serial.println("Disconnect");
    reconnect();
  }

  int packetLen; 
  packetLen=LoRa.parsePacket();
  if(packetLen==16){
    y = 0; 
    while (LoRa.available()){
      rdp.frame[y]=LoRa.read(); 
      y++;
    }
  d1=rdp.data[0]; 
  d2=rdp.data[1];
  int rssi=LoRa.packetRssi(); //Force du signal en réception en dB
  Serial.println(d1); 
  Serial.println(d2); 
  Serial.println(rssi); 
  }

  client.loop();
}

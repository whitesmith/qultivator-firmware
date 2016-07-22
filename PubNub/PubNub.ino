#include <ESP8266WiFi.h>
#include "./passwords.h"
#include <PubNub.h>

char pubkey[] = "<insert-key-here>";
char subkey[] = "<insert-key-here>";
char origin[] = "pubnub.ably.io";
const static char channel[] = "arduino";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
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


  PubNub.begin(pubkey, subkey, origin);
  Serial.println("PubNub set up");

}


void loop() {
  
  WiFiClient *client;
   
  char msg[] = "\"Yo!\"";

  client = PubNub.publish(channel, msg);

  if (!client) {
    Serial.println("publishing error");
    delay(1000);
    return;
  }
  client->stop();
  
  // wait to fully let the client disconnect
  delay(3000);
  
}

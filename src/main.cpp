#include <Arduino.h>
#include <ESPAsyncE131.h>
#include "credentials.h"

#define UNIVERSE 75
#define UNIVERSE_COUNT 1
#define RELAY_PIN 4

ESPAsyncE131 e131(UNIVERSE_COUNT);

void wifi_setup();

void setup() {
    Serial.begin(115200);
    delay(10);
    
    pinMode(RELAY_PIN, OUTPUT);

    wifi_setup();

    if (e131.begin(E131_UNICAST))
        Serial.println(F("Listening for data..."));
    else 
        Serial.println(F("*** e131.begin failed ***"));
}

void loop() { 
    if (!e131.isEmpty()) {
        e131_packet_t packet;
        e131.pull(&packet);

        bool relay_state = packet.property_values[1] > 127;
        
        Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH1: %u / Relay: %s\n",
                htons(packet.universe),                 // The Universe for this packet
                htons(packet.property_value_count) - 1, // Start code is ignored, we're interested in dimmer data
                e131.stats.num_packets,                 // Packet counter
                e131.stats.packet_errors,               // Packet error counter
                packet.property_values[1],              // Dimmer data for Channel 1
                relay_state ? "High" : "Low");          // What we're doing with the relay
      
        digitalWrite(RELAY_PIN, relay_state);
    }
}

void wifi_setup() {
    WiFi.mode(WIFI_STA);

    Serial.println("");
    Serial.print(F("Connecting to "));
    Serial.print(ssid);

    WiFi.begin(ssid, passphrase);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print(F("Connected with IP: "));
    Serial.println(WiFi.localIP());
    
}
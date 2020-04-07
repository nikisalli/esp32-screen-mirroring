#include "SPI.h"
#include "SSD_13XX.h" //https://github.com/sumotoy/SSD_13XX
#include <WiFi.h>

SSD_13XX tft = SSD_13XX(5, 16, 17);
WiFiClient client;

const char* ssid = "ssid";
const char* password =  "password";
 
const uint16_t port = 8090;         // esp32 port
const char * host = "192.168.1.12"; // esp32 local IP
 
void setup(){
    Serial.begin(115200);
 
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("...");
    }
 
    Serial.print("WiFi connected with IP: ");
    Serial.println(WiFi.localIP());
}
 
void loop(){
    if (!client.connect(host, port)) {
 
        Serial.println("Connection to host failed");
 
        delay(1000);
        return;
    }

	unsigned int bytecount;

    while(1){
		unsigned long time = millis();
    	while((millis() - time) < 1000){
			if(client.available()){
				client.read();
				bytecount++;
			}
		}
		Serial.println(String(bytecount/500.0) + " kBps");
		bytecount = 0;
    }
}
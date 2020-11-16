
// This is the program that receives the data and turns the servo.

//=============

#include <ESP8266WiFi.h>
#include<SPI.h>
#include <Servo.h>


    #include <espnow.h>
   //  #include <user_interface.h>

uint8_t mac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x33};

//==============

void initVariant() {
  WiFi.mode(WIFI_AP);
  wifi_set_macaddr(SOFTAP_IF, &mac[0]);
}

//==============

#define WIFI_CHANNEL 4

    // must match the controller struct
struct __attribute__((packed)) DataStruct {
    int y;
    int x;
    //char text[32];
    unsigned int time;
};

DataStruct myData;

Servo servo1;  // create servo object to control a servo
Servo servo2;

int Xval, Yval;

//============

void setup() {
  
    servo1.attach(D5);
    servo2.attach(D6);
    Serial.begin(115200); 
    Serial.println();
    Serial.println("Starting EspnowSlave.ino");

    Serial.print("This node AP mac: "); Serial.println(WiFi.softAPmacAddress());
    Serial.print("This node STA mac: "); Serial.println(WiFi.macAddress());

    if (esp_now_init()!=0) {
        Serial.println("*** ESP_Now init failed");
        while(true) {};
    }

    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);

    esp_now_register_recv_cb(receiveCallBackFunction);


    Serial.println("End of setup - waiting for messages");
    
}

//============

void loop() {

}

//============

void receiveCallBackFunction(uint8_t *senderMac, uint8_t *incomingData, uint8_t len) {
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("NewMsg ");
    Serial.print("MacAddr ");
    for (byte n = 0; n < 6; n++) {
        Serial.print (senderMac[n], HEX);
    }
   Serial.println();
    move();
    Serial.println(myData.x);
    Serial.println(myData.y);
    Serial.println();
}

void move()
{ 
  Xval = myData.x ;//- 7;
  Yval = myData.y ;// - 6;
    
  servo1.write(Xval);
  delay(15);
  servo2.write(Yval);
  delay(15);
  
  Serial.print(" Xaxis = "); Serial.print(Xval); // raw data with offset
  Serial.print(" Yaxis = "); Serial.println(Yval); // raw data with offset
  
 }

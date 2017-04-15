#include <Pushover.h>
#include <ESP8266WiFi.h>
#define BUTTON 4
int buttonstate ;
int lastbuttonstate= LOW;
unsigned long lastdebouncetime = 0;
unsigned long debouncedelay = 50;
const char* ssid = "ssid";
const char* password = "password";

Pushover po = Pushover("apptoken","usertoken");
WiFiClient espClient;
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);
  setup_wifi();
}
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
void loop() {
  int reading = digitalRead(BUTTON);
if(reading != lastbuttonstate) {
lastdebouncetime = millis();
}
if((millis() - lastdebouncetime) >debouncedelay) {
if(reading != buttonstate) {
buttonstate = reading;
  if (buttonstate == LOW) {
    Serial.println("pressed");
    po.setMessage("NEED HELP!!!!!");
    Serial.println(po.send());
    delay(500);
  }
}
}
lastbuttonstate = reading;
}

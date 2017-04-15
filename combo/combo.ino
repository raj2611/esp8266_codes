#include <Pushover.h>
#include <ESP8266WiFi.h>
#define BUTTON 4
int buttonstate ;
int lastbuttonstate= LOW;
unsigned long lastdebouncetime = 0;
unsigned long debouncedelay = 50;
int ledPin = 12;
const char* ssid = "ssid";
const char* password = "password";

Pushover po = Pushover("apptoken","usertoken");
WiFiClient espClient;
WiFiServer server(80);

void setup() {
 Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);
  io(); 
}
void io() {
  
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
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

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  iotlight();
  
  push();

  delay(10);
  }

void iotlight() {
// Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
  if (request.indexOf("/LED=FLASH") != -1) {
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a se
  }
  // Set ledPin according to the request
  //digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Led pin is now: ");

  if (value == HIGH) {
    client.print("On");
  } else if (value == LOW) {
    client.print("Off");
  } else {
    client.print("flash");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"><button>ON</button></a>&nbsp;<a href=\"/LED=OFF\"><button>OFF</button></a>&nbsp;<a href=\"/LED=FLASH\"><button>FLASH</button></a></p>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
  
}

void push() {
  int reading = digitalRead(BUTTON);
if(reading != lastbuttonstate) {
lastdebouncetime = millis();
}
if((millis() - lastdebouncetime) >debouncedelay) {
if(reading != buttonstate) {
buttonstate = reading;
  if (buttonstate == LOW) {
    Serial.println("pressed");
    po.setMessage("NEED help!!!!");
    Serial.println(po.send());
    delay(100);
  }
}
}
lastbuttonstate = reading;
  
}

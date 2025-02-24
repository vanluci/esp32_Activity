#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Van";
const char* password = "12345678";

WebServer server(80);  //  a web server on port 80

const int ledPins[] = {22, 21, 19, 5, 16};
bool allOn = false;

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>LED CONTROLLER</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<style>";
  html += "body { font-family: 'Segoe UI', sans-serif; background: black;";
  html += "display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0;}";
  html += ".container { padding: 30px; width: 90%; max-width: 400px; text-align: center;}";
  html += "h1 { color: #333; font-size: 28px; margin-bottom: 20px;}";
  html += ".btn { background: #ED7014; color: white; border: none; padding: 10px 20px;";
  html += "margin: 10px 0; cursor: pointer; font-size: 16px;";
  html += "transition: 0.3s; box-shadow: 0 8px 15px rgba(0,123,255,0.2);}";
  html += ".btn:hover { background: #ED7014; box-shadow: 0 15px 20px rgba(0,123,255,0.4);";
  html += "transform: translateY(-5px);}";
  html += "</style></head><body>";
  html += "<div class=\"container\">";
  html += "<h1><del>LED CONTROLLER</del</h1>";
  html += "<ul>";
  html += "<a href=\"/LED=ALL\"><button class=\"btn\">All</button></a>";
  html += "<a href=\"/LED=32415\"><button class=\"btn\">Single</button></a>";
  html += "<a href=\"/LED=12345\"><button class=\"btn\">3,2-4,5-1</button></a>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

void handleLED() {
  String request = server.uri();

  if (request.indexOf("/LED=ALL") != -1) {
    Serial.println("Toggle All LEDs");
    allOn = !allOn;
    for (int i = 0; i < 5; i++) {
      digitalWrite(ledPins[i], allOn ? HIGH : LOW);
    }
  } else if (request.indexOf("/LED=32415") != -1) {
    Serial.println("Sequence 312345");
    int sequence[] = {22, 21, 19, 5, 16};
    for (int i = 0; i < 5; i++) {
      digitalWrite(sequence[i], HIGH);
      delay(300);
      digitalWrite(sequence[i], LOW);
    }
  } else if (request.indexOf("/LED=12345") != -1) {
    Serial.println("Sequence 32415");
    one();
    two();
    three();
    two();
    one();
    clr();
  }

  server.sendHeader("Location", "/");
  server.send(303); // redirect back to home page
}

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/LED=ALL", handleLED);
  server.on("/LED=32415", handleLED);
  server.on("/LED=12345", handleLED);

  server.begin();
}

void loop() {
  server.handleClient(); // handle http client req
}

// function to turn off all led
void clr() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  delay(100);
}

// led Sequence functions
void one() {
  clr();
  digitalWrite(19, HIGH);
  delay(300);
}

void two() {
  clr();
  digitalWrite(5, HIGH);
  digitalWrite(21, HIGH);
  delay(300);
}

void three() {
  clr();
  digitalWrite(16, HIGH);
  digitalWrite(22, HIGH);
  delay(300);
}

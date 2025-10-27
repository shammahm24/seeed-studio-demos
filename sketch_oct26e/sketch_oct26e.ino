#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>

#define MIC_PIN D2
#define LED_PIN D10
#define NUMPIXELS 4

const char* ssid = "Network Name";
const char* password = "Network Password";

WebServer server(80);
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

int soundLevel = 0;

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels.setBrightness(50);
  pixels.show();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected. IP: " + WiFi.localIP().toString());

  // Serve main page
  server.on("/", []() {
    String html = R"rawliteral(
      <html>
      <head>
        <title>ESP32 Sound Meter</title>
        <style>
          #bar {
            width: 300px;
            height: 30px;
            background-color: #ddd;
          }
          #level {
            height: 30px;
            background-color: #4CAF50;
            width: 0%;
          }
        </style>
      </head>
      <body>
        <h1>Sound Level</h1>
        <div id="bar">
          <div id="level"></div>
        </div>
        <p id="value">0%</p>

        <script>
          function updateLevel() {
            fetch('/level')
              .then(response => response.text())
              .then(data => {
                let val = parseInt(data);
                document.getElementById('level').style.width = val + '%';
                document.getElementById('value').innerText = val + '%';
              });
          }
          setInterval(updateLevel, 100); // update every 100ms
        </script>
      </body>
      </html>
    )rawliteral";
    server.send(200, "text/html", html);
  });

  // Endpoint to return sound level
  server.on("/level", []() {
    int val = analogRead(MIC_PIN);
    int level = map(val, 0, 4095, 0, 100);
    server.send(200, "text/plain", String(level));
  });

  server.begin();
}

void loop() {
  server.handleClient();

  int val = analogRead(MIC_PIN);
  int level = map(val, 0, 4095, 0, NUMPIXELS);

  for (int i = 0; i < NUMPIXELS; i++) {
    if (i < level) pixels.setPixelColor(i, pixels.Color(0, 255, 0));
    else pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
  delay(50);
}
